/**
    POWERBOX firmware
    (c) 2022 by Boris Emchenko
    HW rev 1.2

    TODO:
      - use status led to indicate some events (critical error, warning error)
      - send data to web server (something like logging events directly from powerbox)
      - storing configuration into flash (emergency halt delay, soft stop wait time)

    
    Changes:
       ver 2.9 2022/04/16 [402301/36464]
           - Status Led more extensive use
           - critical errors handling changed
           - manual reboot
           - added network connection error displaying in web
       ver 2.8 2022/04/16 [401173/36412]
           - SHT31 heater external commands
           - code formating and clean up
       ver 2.7 2022/04/15 [400893/36428]
           - SHT31 heater auto control (on humidty and max off time)
           - Cloud IDX caclulations
       ver 2.6 2022/04/15 [399993/36416, only mlx 397361/36320, only sht31 398513/36388, no sensors 395897/36292]
           - SHT31 support 
           - MLX90614 support
           - configurable presense of these sensor in Configuration.h
           - flexible webpage based on presense of sensors in Configuration.h
       ver 2.5 2022/04/11 [395601/36268]
           - relay on/off state can be specified (not 1 - on, 0 - off, as earlier) in Сonfiguration.h
       ver 2.4 2022/04/09 [395385/36252]
           - optimization release
           - web button interface improved
           - softstop handler improved
           - web command were changed to #define constants
       ver 2.3 2022/04/09 [395257/36252]
           - moved to iotwc events (bug is gone, at least for now)
           - REST API 
       ver 2.2b 2022/04/09 [390292/35532]
           - using #define commands also in WebPage
           - pwm input doesn't overwritten by update during editing
           - at once tested :)
           - bug found and temporary commented out (to deal with later): esp rebooting on wifi connect up to 10 times
       ver 2.2 2022/04/04 [390292/35532] NOT TESTED!
           - using #define commands in OUTPUT
           - some web update (JSON) name sync with serial (need to test!)
       ver 2.1 2022/03/29 [389968/35468]
           - USB web control: display and toggle
           - web some optimization
           - totally rewritten SoftStop and EmergencyHalt routines
           - modified web for this
       ver 2.0b 2022/03/28 [388708/35524]
           - some minor optimization
       ver 2.0 2022/03/25 [388600/35544]
           - goes to AP mode if couldn't connect to WiFi (based on IotWebConf)
           - code optimization
           - web optimization
       ver 1.7b 2022/03/25 [367136/33176]
           - non blocking wifi connection/reconnection
           - button status handling commands
           - controlling button status from web
           - Emergency Halt flag in web
       ver 1.6 2022/03/22 [365412/33084]
           - INA self check
           - web error displaying
           - more network data to web
           - some optimization
       ver 1.5 2022/03/21 [364536/33184]
           - web commands moved to string commands (same as serial)
           - led controlling code optimization
           - led status web indicating
       ver 1.4 2022/03/21 [366572/33468]
           - PWM web controlling
           - commands throug web processing
       ver 1.3 2022/03/21 [362836/33124]
           - PWM controlling through serial
           - serial comands coded through define constants
       ver 1.2 2022/03/21 [362836/33124]
           - many INA support
           - some web reformating
       ver 1.1 2022/03/16 [360928/32824]
           - 2nd relay support
       ver 1.05 2022/02/15 [361461/33628]
           - code restructuring and optimizing
           - output pwm data (not finished)
       ver 1.04 2022/02/13 [361253/33588]
           - set relay from serial
           - debug changed to #ifdef directive. Now all memory data would be specified with DEBUGF, but not DEBUGF_MORE
       ver 1.03 2022/02/10 [360753/33440]
           - output relay data (not finished)
       ver 1.02 2022/02/10 [360753/33440]
           - output network data
       ver 1.01 2022/02/07 [360065/33376]
           - emergency halt routine drafted
       ver 1.0 2022/02/07 [359857/33340]
           - first run on hardware rev1.2
           - some tunning for new hw rev
           - self check changed
           - (a) button indication enhanced
       ver 0.9c 2022/01/29 [359297/33308]
           - minor bug fix
       ver 0.9b 2022/01/27 [358961/33308]
           - code optimization and structuring
       ver 0.9a 2022/01/26 [358961/33308]
           - self check for INA
           - code optimization
       ver 0.9 2022/01/26 [358889/33304]
           - INA reading
       ver 0.8 2022/01/26 [356189/33132]
           - main led blinking (ticker)
           - more button pressing events
           - button led blinking (ticker)
       ver 0.7 2022/01/26 [355773/33092]
           - button events creating and handling
           - code structuring
       ver 0.6 2022/01/24 [352365/32436]
           - serial input parsing
           - self check
       ver 0.5 2022/01/16 [352773/32312]
           - some code optimization (intermediate version, not finished)
       ver 0.4 2022/01/16 [351457/32232]
           - added led button
           - some optimization
       ver 0.3 2022/01/16 [351105/32232]
           - added relay switch handling
           - added color switch handling
       ver 0.2 2022/01/16 [347685/32068]
           - added webserver
           - added OTA
           - added proto relay handling
       ver 0.1 2022/01/16 [269793/28516]
           - forming framework
           - added MCP support
           - added RGB led indicator support
*/

//Compile version
#define VERSION "2.9"
#define VERSION_DATE "20220416"
 
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
MCP23017          mcp2                        = MCP23017(MCP_2_ADDR);


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


/* Main led variables */
uint8_t           MainLed_Status              = LOW;              // Current status for Main LED (ON or OFF)
Colors_Enum       MainLed_Color               = White;            // Current color used for Main LED, for convinience. Command LED_ON uses separate color status vars (below)
uint8_t           MainLed_RedStatus           = LOW;              // Will (or already) RED led be used for current color
uint8_t           MainLed_GreenStatus         = LOW;              // Will (or already) GREEN led be used for current color
uint8_t           MainLed_BlueStatus          = LOW;              // Will (or already) BLUE led be used for current color
Ticker            MainLedTicker;                                  // Ticker for LED blinking


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
    Serial.println(F("POWER BOX"));
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

    mcp2.portMode(MCP23017Port::A, 0);                          //Port A as output (0b11111111 as input, next parameter pullups)
    mcp2.portMode(MCP23017Port::B, 0);                          //Port B as output
    mcp2.writeRegister(MCP23017Register::GPIO_A, 0x00);         //Set all pins (0-7) to LOW
    mcp2.writeRegister(MCP23017Register::GPIO_B, 0x00);         //Set all pins (8-15) to LOW
    mcp2.digitalWrite(MCP2_TEST_PIN, HIGH);                     //Should left HIGH forever. Using for test MCP workging status

    mcp2.pinMode(BUTTON_PIN, INPUT);

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

    /* Read button events */
    if ((currentTime - _lastGetResult_BUTTON) > BUTTON_EVENTS_ACCUMULATE_INTERVAL) {
      EVENT_Button_Pressing = readButton_result();  // current event
      Calculate_Button_Events();
    } else {
      // continue accumulate async results
      readButton_async();
    }

    /* hadle button events */
    Handle_Button_Events();


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
    Relay2StatusFlags = Relay2StatusAll();

   
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

    /* clear all events in the end of the cycle */
    Clear_Events();

}
