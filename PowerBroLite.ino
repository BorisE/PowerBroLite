/**
    POWERBRO firmware
    (c) 2022 by Boris Emchenko
    HW rev 2.0 lite

    TODO:
      - initial relay values doesn't work when set to HIGH
      - code optimization after removing unused blocks (relay block, button vars and emergency routines)
      - return something like "main led" (to HW and then to code)
      - return PWM block
    
    Changes:
       ver 0.3a 2022/08/24 [386873/35824]
       - change PowerBox to PowerBro everywhere
       ver 0.3 2022/08/24 [386853/35824]
       - web page uodated to lite 
       - selfcheck updated
       - some more code cleanup
       ver 0.2 2022/08/22 [391937/35836]
       - some more cleanup. Code runs!
       ver 0.1 2022/08/22 [388028/35436]
       - starting from fw 2.9 402301/36464
       - removed button events and all stuff
       - removed main led routines
       - one MCP, and one relay block
       - all exept one INA
*/

//Compile version
#define VERSION "0.3a"
#define VERSION_DATE "20220824"
 
#include <Ticker.h>
#include <Wire.h>
#include <MCP23017.h>
#include <GyverINA.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoOTA.h>
#include <ESP8266HTTPUpdateServer.h>
#include <IotWebConf.h>
#include <Adafruit_SHT31.h>
#include <MLX90614.h>

//#include <FS.h>

#include "Configuration.h"
#include "MiscFunctions.h"

ESP8266WebServer  server(80);
ESP8266HTTPUpdateServer httpUpdater;

DNSServer         dnsServer;

IotWebConf        iotWebConf(APName, &dnsServer, &server, APPass, IOTWC_CONFIG_VERSION);

MCP23017          mcp1                        = MCP23017(MCP_1_ADDR);


/* SENSORS VARS */
uint8_t           Button_Status               = 0;                // Button sensor status
Ticker            EmergencyHaltTicker;                            // Button sensor - emergeny halt started
Ticker            SoftStopStartTicker;                            // Button sensor - emergeny halt started
uint8_t           EmergencyHalt_Fired         = 0;                // Emergency halt routine was run
uint8_t           SoftStop_NeedToStart        = 0;                // SoftSoft rountine need to be started
uint8_t           SoftStop_WasRun             = 0;                // SoftSoft rountine was correctly finished

/* RELAY VARS */
uint8_t           Relay1StatusFlags           = 0b0;              // relay status in binary form
uint8_t           Relay2StatusFlags           = 0b0;              // relay status in binary form

/* PWM VARS */
uint16_t          PWM1_Level                  = 0;                // PWM level for MOSFET 1
uint16_t          PWM2_Level                  = 0;                // PWM level for MOSFET 2
uint16_t          PWM3_Level                  = 0;                // PWM level for MOSFET 3

/* INA VARS */
float             ina_volt[INA_COUNT];
float             ina_cur[INA_COUNT];
float             ina_pwr[INA_COUNT];
unsigned long     _lastGetResult_INA          = 0;                // need to be global

/* SENSORS */
#ifdef ENABLE_SHT31
  Adafruit_SHT31  sht31                       = Adafruit_SHT31();
  float           SHT_T                       = TEMPERATURE_UNKNOWN_VALUE;
  float           SHT_H                       = TEMPERATURE_UNKNOWN_VALUE;
  uint8_t         SHT_HeaterStatus            = SHT_HEATER_OFF;
  uint8_t         SHT_Heater_need_to_switch_ON= 0;
  uint16_t        SHT_ReadErrorsCount         = 0;                // continious receive errors count
  unsigned long   _lastSHTHeaterON            = 0;                // need to be global
  unsigned long   _lastSHTHeaterOFF           = 0;                // need to be global
#endif
#ifdef ENABLE_MLX90614
  MLX90614        mlx                         = MLX90614(MLX90614_I2C_ADDR);
  float           MLX_obj                     = 0.0;
  float           MLX_amb                     = 0.0;
  float           MLX_cloud                   = TEMPERATURE_UNKNOWN_VALUE;  
#endif
uint8_t           currentReadingSensor        = 0;


/* Status Led variables */
Ticker            StatusLedTicker;                                // Ticker for LED blinking

/* Button events */
bool              EVENT_Button_Pressing                    = false;            // true while button is pressed
bool              EVENT_Button_Pressing_Long_Reached       = false;            // true while button is pressed and long threshold is reached
bool              EVENT_Button_Pressing_VeryLong_Reached   = false;            // true while button is pressed and very long threshold is reached
bool              EVENT_Button_Pushed                      = false;            // true after button was pressed
bool              EVENT_Button_Released                    = false;            // true after button was released
bool              EVENT_Button_Pressed_Short               = false;            // true if button was pressed for short
bool              EVENT_Button_Pressed_Long                = false;            // true if button was pressed for long
bool              EVENT_Button_Pressed_VeryLong            = false;            // true if button was pressed for very long
Ticker            ButtonLedTicker;                                           // Ticker for Button led blinking
unsigned long     _lastGetResult_BUTTON       = 0;                // need to be global

/* ERROR CHECKING */
uint8_t           hardwareERRstatus           = 0;
String            hardwareERRstring           = "";
uint8_t           checkStatusMCP1             = 0;                // current status of MCP1(0 -ok, 1 - error)
uint8_t           checkStatusMCP2             = 0;                // current status P1
uint8_t           checkStatusINA              = 0;                // current status of INA sensors
uint8_t           checkStatusMLX              = 2;                // current status of MLX sensor
uint8_t           checkStatusSHT              = 2;                // current status of SHT sensor
uint8_t           checkStatusMEM              = 0;                // current current memory
uint16_t         _CriticalError_detected      = 0;                // critical errors count


unsigned long     currentTime                 = 0;

String            serialString                = "";               // a String to hold serial incoming data
bool              serialStringComplete        = false;            // whether the string is complete

/***********1*********************************************
*     SETUP
********************************************************/
void setup() {
    // Wait for serial to initialize.
    Serial.begin(115200);
    while(!Serial) { }
    while((millis()) < 3100) {
      Serial.print(".");
      delay (500);
    }
    Serial.println();

    printBoardInfo();

    // Greeting message
    Serial.println(F("POWER BRO LITE"));
    Serial.print ("v");
    Serial.print (VERSION);
    Serial.print (" [");
    Serial.print (VERSION_DATE);
    Serial.println ("]");

    
    ////////////////////////////////
    // START HARDWARE
    pinMode(STATUS_LED, OUTPUT);

    Wire.begin();

    mcp1.init();
    mcp1.portMode(MCP23017Port::A, 0);                          //Port A as output (0b11111111 as input, next parameter pullups)
    mcp1.portMode(MCP23017Port::B, 0);                          //Port B as output
    mcp1.writeRegister(MCP23017Register::GPIO_A, 0x00);         //Set all pins (0-7) to LOW
    mcp1.writeRegister(MCP23017Register::GPIO_B, 0x00);         //Set all pins (8-15) to LOW
    mcp1.digitalWrite(MCP1_TEST_PIN, HIGH);                     //Should left HIGH forever. Using for test MCP workging status

    // INA219 initialization
    for(int i=0; i<INA_COUNT; i++)
    {
      INA_OBJ[i].begin();
      Serial.print(F("INA["));
      Serial.print(i);
      Serial.print(F("] calibration value: ")); 
      Serial.println(INA_OBJ[i].getCalibration()); 
      INA_OBJ[i].setResolution(INA219_VBUS, INA219_RES_12BIT);      // Напряжение в 12ти битном режиме + 4х кратное усреднение
      INA_OBJ[i].setResolution(INA219_VSHUNT, INA219_RES_12BIT);    // Ток в 12ти битном режиме + 128х кратное усреднение
    }
    
    // PWM
    pinMode(PWM1_PIN, OUTPUT);
    pinMode(PWM2_PIN, OUTPUT);
    pinMode(PWM3_PIN, OUTPUT);
    setPWM(1,0);
    setPWM(2,0);
    setPWM(3,0);

    // SHT31
    #ifdef ENABLE_SHT31
      if (! sht31.begin(SHT_ADDR)) {
        Serial.println(F("Couldn't find SHT31"));
      }
    #endif

    // MLX90614
    #ifdef ENABLE_MLX90614
      mlx.begin(); 
    #endif

    // First selfcheck
    selfCheckHardware();
    
    ////////////////////////////////
    // WiFI managent part
    /* Starting iotWebConf */
    iotWebConf.setStatusPin(STATUS_LED);            // register led
    iotWebConf.skipApStartup();                     // skip creating AP on each startup
    iotWebConf.setWifiConnectionTimeoutMs(IOTWC_WAIT_FOR_WIFI_CONNECTION);   // time for attempting to reconnect before popup config portal
    iotWebConf.setWifiConnectionCallback(&Network_Connected);

    iotWebConf.init();                              // start

    /* Setup WebServer routing */
    server.on("/",          handleRoot);
    server.on("/json",      handleJSON);
    server.on("/ping",      handlePingRequest);
    server.on("/cmd",       handleCommands);
    server.on("/REST/relay/", HTTP_GET, handleREST_GET);
    server.on("/REST/relay/", HTTP_POST, handleREST_POST);

    server.on("/config", []{ iotWebConf.handleConfig(); });
    server.onNotFound([](){ iotWebConf.handleNotFound(); });
    // and don't forget about /update for OTA - it is set in Network_Connected

    ////////////////////////////////
    // Final setup actions    
    #ifdef DEBUGF
    Serial.println("[DEBUG ON]");
    #endif
    #ifdef DEBUGF_MORE
    Serial.println("[DEBUG MORE ON]");
    #endif
    
    Serial.println(F("==========================="));
    Serial.println(F("Setup complete."));
    Serial.println(F("==========================="));
}

/********************************************************
*     LOOP
********************************************************/
void loop() {
    static unsigned long _lastSensorsOftenPrint = 0, _lastSensorsRarePrint = 0, _lastCheckHardware =0;
    currentTime = millis();

    server.handleClient();  //Serial.println("Debug 1");
    MDNS.update();          //Serial.println("Debug 2");
    ArduinoOTA.handle();    //Serial.println("Debug 3");
    iotWebConf.doLoop();    //Serial.println("Debug 4");
  
    /* Check Serial input */
    while (Serial.available()) {
      char inChar = (char)Serial.read();
      if (inChar == '\n' || inChar == '\r') {
        serialStringComplete = true;
      } else {
        serialString += inChar;
      }
    }
    
    /* Parse serial input when the sting is complete */
    if (serialStringComplete) {
      #ifdef DEBUGF_MORE
        Serial.print (F("Input string: '"));
        Serial.print (serialString);
        Serial.println ("'");
      #endif
      parseInputCommands(serialString);
      serialStringComplete = false;
      serialString = "";
    }


    /* Read INA data and SENSORS */
    if ((currentTime - _lastGetResult_INA) > INA_DATA_ACCUMULATE_INTERVAL) {
      readINA_result();

      // Read sensors - only one at a time and when no readings of INA occurs (alternating sensors each cycle)
      if (currentReadingSensor == 1) {
        #ifdef ENABLE_SHT31
          ReadSHT();
        #endif
      } else if (currentReadingSensor == 2) {
        #ifdef ENABLE_MLX90614
          ReadMLX();
        #endif
      }
      currentReadingSensor++;
      if (currentReadingSensor > NUMBER_OF_SENSORS) currentReadingSensor = 0;
      
    } else {
      // continue accumulate async results
      readINA_async();
    }


    /* read relay status */
    Relay1StatusFlags = Relay1StatusAll();

   
    /* Print sensor status */
    if ( currentTime - _lastSensorsOftenPrint > SENSORS_DATA_PRINT_OFTEN_INTERVAL ) {
        SensorsSerial_Often();
        _lastSensorsOftenPrint = currentTime;
    }
    if ( currentTime - _lastSensorsRarePrint > SENSORS_DATA_PRINT_RARE_INTERVAL ) {
        SensorsSerial_Rare();
        _lastSensorsRarePrint = currentTime;
    }

    /* Regular self check */
    if ( currentTime - _lastCheckHardware > CHECK_HARDWARE_STATUS_INTERVAL) {
        selfCheckHardware();
        Output_Selfcheck_Results();
        if (hardwareERRstatus == SELFCHECK_CRITICAL_ERROR) {
          _CriticalError_detected++;
          selfCheck_HandleCrtiticalError();
        } else {
          _CriticalError_detected = 0;
        }
        _lastCheckHardware = currentTime;
    }

}
