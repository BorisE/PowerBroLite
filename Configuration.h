//#include <WiFiCredentials.h>            // become obsolete with IOTWC
/* for Wemos D1 R2 and D1 Mini
 *  
#define PIN_WIRE_SDA (D2)    4   
#define PIN_WIRE_SCL (D1)    5   
#define LED_BUILTIN 2       D4

static const uint8_t D0   = 16; /no interrupts, HIGH at boot                                          
static const uint8_t D1   = 5;  /scl                                                                    
static const uint8_t D2   = 4;  /sda                                                                    
static const uint8_t D3   = 0;  /10k pullup, boot fails if pulled LOW; connected to FLASH button  
static const uint8_t D4   = 2;  /10k pullup, boot fails if pulled LOW; Buit-in LED                    <-яркий диод
static const uint8_t D5   = 14; /+, built in led2 ("SCK")                                             
static const uint8_t D6   = 12; /+                                                                    
static const uint8_t D7   = 13; /+                                                                    
static const uint8_t D8   = 15; /10k pulldown, Boot fails if pulled HIGH                              <-при загузке скетча использует этот пин. потом можно использовать; для реле не подходит

GPIO6-GPIO11 - flash pins
*/
/*************************************************************
* CONFIGURATION
*************************************************************/
//#define ENABLE_MLX90614                                                       // comment this line out if MLX90614 sensor not present
//#define ENABLE_SHT31                                                          // comment this line out if SHT31 sensor not present
#define NUMBER_OF_SENSORS                           0                         // correct this when commenting out sensor 
/*************************************************************
* I2C SETTINGS
*************************************************************/
/* I2C Adresses */
#define MCP_1_ADDR                                  0x20

#define INA_MAIN_ADDR                               0x40

#define SHT_ADDR                                    0x44                      // same as MOSFET addr and this give us a problem :(
#define MLX90614_I2C_ADDR                           0x5A

/*************************************************************
* PINS SETTINGS
*************************************************************/
/* wemos pins */
#define STATUS_LED                                  D4

// MOSFET pins
#define PWM1_PIN                                    D5                        // MOSFET 1 PWM Pin
#define PWM2_PIN                                    D7                        // MOSFET 2 PWM Pin
#define PWM3_PIN                                    D6                        // MOSFET 3 PWM Pin

/* MCP pins*/
// Self test pins
#define MCP1_TEST_PIN                               7                         // test pin for testing of connection


// Relay PINs
#define RELAY1_CH1_PIN                              8                         // on MCP, change it name in functions
#define RELAY1_CH2_PIN                              9                         // on MCP, change it name in functions
#define RELAY1_CH3_PIN                              10                         // on MCP, change it name in functions
#define RELAY1_CH4_PIN                              11                         // on MCP, change it name in functions
#define RELAY1_CH5_PIN                              12                         // on MCP, change it name in functions
#define RELAY1_CH6_PIN                              13                         // on MCP, change it name in functions

/*************************************************************
* DEBUG CONFIGURATION
*************************************************************/
#define DEBUGF                                                                // comment to remove debug sections
//#define DEBUGF_MORE                                                         // comment to remove extensive debug sections 

/*************************************************************
* RELAY SETTING
*************************************************************/
// which state correspondes to ON (CLOSED) state of relay1
uint8_t RELAY1_ON_STATE[7] {
    0,                                                                        // DUMMY element (array index starts from 0)
    LOW,                                                                      // Channel 1
    LOW,                                                                      // Channel 2
    LOW,                                                                      // Channel 3
    LOW,                                                                      // Channel 4
    LOW,                                                                      // Channel 5
    HIGH,                                                                     // Channel 6
};

/*************************************************************
* INA SENSOR SETTING
*************************************************************/
#define INA_COUNT 1
INA219 INA_OBJ[] {
    INA219(0.00909f,  10.0f,  INA_MAIN_ADDR),
};
//INA219 ina(0.035f, 2.0f, 0x41);   //def (0.1, 3.2, 0x40) 0.1R/3.2A/0x40 ADDR


/*************************************************************
* CALCULATIONS INTERVAL SETTINGS
*************************************************************/
#define JS_UPDATEDATA_INTERVAL                      10000                     // how often to update webpage, default 10000

#define SENSORS_DATA_PRINT_OFTEN_INTERVAL           5000                      // how often sensor data printed to Serial
#define SENSORS_DATA_PRINT_RARE_INTERVAL            10000                     // how often sensor data printed to Serial

#define BUTTON_EVENTS_ACCUMULATE_INTERVAL           100                       // how long accumulate read events (debounce lag)
#define BUTTON_LONG_PRESS_INTERVAL                  2000                      // for how long to press button for long press event
#define BUTTON_VERY_LONG_PRESS_INTERVAL             8000                      // for how long to press button for very long press event

#define INA_DATA_ACCUMULATE_INTERVAL                1000                      // how long accumulate ina data (for averaging out noise). Also how ofter to read SHT and MLX data (turn by turn - one at a time). Made this because SHT and one of INA shares the same I2C addres (my fault)

#define POWER_HALT_EMERGENCY_START_DELAY            10000                     // for how long wait when button was pressed VERY LONG before initiating EMERGENCY HALT ROUTINE. During this interval you can cancel this action by pressing Button
#define POWER_HALT_SOFTSTOP_START_DELAY             10000                     // for how long wait when button was pressed LONG before SOFT STOP ROUNTING started. During this interval you can cancel this action by pressing Button
#define POWER_HALT_SOFTSTOP_FULL_HALT_DELAY         60000                     // for how long wait after softstop initiated to complete and clear button flag. If Button flag wouldn't be cleared, emergency halt routine would be run

/*************************************************************
*  Network config
*************************************************************/
#define OTA_PORT                                    18266                     // OTA PORT

const char* host                                    = "powerbrolite";         // used in MNDS and update server
const char* APName                                  = "PowerBro";             // IOTWC AP name when no wifi connection is found
const char* APPass                                  = "12345678";             // IOTWC AP password
#define IOTWC_CONFIG_VERSION                        "v3"                      // Config version to store in EEPROM by IOTWC.
#define IOTWC_WAIT_FOR_WIFI_CONNECTION              20000                     // How long wait when there is no wifi connection before creating AP, msec (give a chance to reconnect)

/*************************************************************
*  Some rarely changed config data, but nevertheless
*************************************************************/
#define SENSOR_OUT_START                            "[!"                      // begining of sensor string
#define SENSOR_OUT_END                              "]"                       // ending of sensor string

#define SERIAL_COMMAND_LENGTH                       3                         // all commands should me that length (wrapped in SERIAL_COMMAND_START and SERIAL_COMMAND_END patterns), e.g. "MS1": [?MS1=255]
#define SERIAL_COMMAND_START                        "[?"                      // begining of input command string
#define SERIAL_COMMAND_START_LENGTH                 2                         // for performance reasons hardcode it
#define SERIAL_COMMAND_END                          "]"                       // ending of input command string
#define SERIAL_COMMAND_END_LENGTH                   1                         // for performance reasons hardcode it
#define SERIAL_COMMAND_VALUE_SIGN                   '='                       // char to separate command name and command value

/*************************************************************
*  SELF CHECK Status constants
*************************************************************/
#define CHECK_HARDWARE_STATUS_INTERVAL              10000                     // how often check hardware integrity (should be less then status led twoblinks
#define MAX_CRITICALERRORS_BEFORE_REBOOT            10                        // how many critical errors accumulated before reboot. To translate into time multiply by CHECK_HARDWARE_STATUS_INTERVAL

#define SHT31_CONTINIOUS_ERRORS_TOLERANCE           30                        // how many continiously errors could be received before it becomes treated as a malfunction

#define SELFCHECK_OK                                0
#define SELFCHECK_MINOR_ERROR                       1                         // Minor error, just for information, notice, purpose
#define SELFCHECK_WARNING_ERROR                     2                         // Major but handleable error: some functions are not working, but some core features are still possible
#define SELFCHECK_CRITICAL_ERROR                    10                        // Critical error, nothing is working

/*************************************************************
*  SENSORS constants
*************************************************************/
#define TEMPERATURE_UNKNOWN_VALUE                   -99.9
#define TEMPERATURE_MIN_VALUE                       -99.9
#define TEMPERATURE_MAX_VALUE                       99.9
#define HUMIDITY_MIN_VALUE                          0.9
#define HUMIDITY_MAX_VALUE                          99.9

#define SHT_HEATER_OFF                              0
#define SHT_HEATER_ON                               1
#define SHT_HEATER_COOLDONW                         2

#define SHT_HEATER_KEEP_ON_INTERVAL                 180000                    // how long keep SHT heater on
#define SHT_HEATER_WAIT_COOLDOWN_INTERVAL           120000                    // how long wait before SHT cool down
#define SHT_HEATER_MINIMUM_OFF_INTERVAL             300000                    // minimum wait time before switching heater again
#define SHT_HEATER_MAXIMUM_OFF_TIME_INTERVAL        6*3600*1000               // switch heater at least this interval
#define SHT_HEATER_HUMIDITY_THRESHOLD_TO_SWITCH_ON  80.0                      // Humidity value to switch heater on


#define CLOUD_SENSOR_CLOUDY_THRESHOLD               -10                       // less then this values is VERY CLOUDY, greater - CLOUDY
#define CLOUD_SENSOR_CLEAR_THRESHOLD                -20                       // less then this values is CLOUDY, greater - CLEAR

/*************************************************************
*  Some variables declaration
*************************************************************/
char debugstack[2048];                                                        //buffer to store debug messages befor outputing to web. Not implemented now

/*************************************************************
*  Some types declaration
*************************************************************/
enum Colors_Enum {
  Red,                  // 0
  Green,                // 1
  Blue,                 // 2
  Yellow,               // 3
  Cyan,                 // 4
  Magenta,              // 5
  White,                // 6
  Led_Off               // 7
};
