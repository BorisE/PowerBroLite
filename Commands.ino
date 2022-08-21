/***********************************************************
  Format:
    ?###          run command without parameters (e.g. ?CLB)
    ?###=#...     run command with parameters (e.g. ?R22=0)
    ie always starts from ? sign and has 3 symbols
        
  Commands list:

 ************************************************************/
#define CMD_SWITCH_RELAY                    "R"
#define CMD_SWITCH_RELAY1                   "R1"                      // R11=1:  switch relay 1 on block 1 on/off (1|0) (R11=1 .. R16=1)
#define CMD_SWITCH_RELAY2                   "R2"                      // R21=1:  switch relay 1 on block 2 on/off (1|0) (R21=1 .. R26=1)

#define CMD_PWM_LEVEL                       "PM"
#define CMD_PWM1_LEVEL                      "PM1"                     // PM1=100: set PWM value for out1 to 100 (0..1023)
#define CMD_PWM2_LEVEL                      "PM2"
#define CMD_PWM3_LEVEL                      "PM3"

#define CMD_LED_SWITCH                      "LED"                     // LED=1: switch led on/off (1|0) with last used color
#define CMD_LED_COLOR                       "LEC"                     // LEC=0..7: set led color and swith it on (0 = Red, 1=Green, 2=Blue, 3=Yellow, 4=Cyan, 5=Magenta, 6=White, 7=Led_Off)

#define CMD_BTN_SHORTPESS                   "BTS"                     // raise button short press event
#define CMD_BTN_LONGPESS                    "BTL"                     // raise button long press event
#define CMD_BTN_VERYLONGPESS                "BTV"                     // raise button very long press event

#define CMD_BTN_CLEAR_STATUS                "CLB"                     // clear button status. if not called, Button_status will always handle last button status
#define CMD_EMERGENCY_HALT_FLAG_CLEAR       "CLE"                     // clear emergency halt flag (EmergencyHalt = 0). If non called, will always store last EmergencyHalt status (1 or 0 strictly speaking)
#define CMD_SOFTSTOP_FINISH_ROUTINE         "SSF"
#define CMD_SOFTSTOP_FLAGS_CLEAR            "CLS"                     // clear Softstop Procedure. Should be run externally at the end of SoftStop Routine

#define CMD_SHT_HEATER_ON                   "SH1"
#define CMD_SHT_HEATER_OFF                  "SH0"

#define CMD_ESP_REBOOT                      "RBT"                     // Reboot ESP


/*
 *  Parse all commands
  */
void parseCommand(String commandNameString, String commandValueString)
{
    if (commandNameString.startsWith(CMD_SWITCH_RELAY1)) {
      Serial.print (F("Command SWITCH RELAY 1 ["));
      Serial.print (commandNameString.substring(2,3));
      Serial.print (F("] received. Set to ["));
      Serial.print (commandValueString);
      Serial.println ("]");
      setRelaySwitch_Str(1, commandNameString.substring(2,3).toInt(),commandValueString.c_str());
    } else if (commandNameString.startsWith(CMD_SWITCH_RELAY2)) {
      Serial.print (F("Command SWITCH RELAY 2 ["));
      Serial.print (commandNameString.substring(2,3));
      Serial.print (F("] received. Set to ["));
      Serial.print (commandValueString);
      Serial.println ("]");
      setRelaySwitch_Str(2, commandNameString.substring(2,3).toInt(),commandValueString.c_str());
  
    } else if (commandNameString.startsWith(CMD_PWM_LEVEL)) {
      Serial.print (F("Command SET PWM ["));
      Serial.print (commandNameString.substring(2,3));
      Serial.print (F("] received. Set to ["));
      Serial.print (commandValueString);
      Serial.println ("]");
      setPWM(commandNameString.substring(2,3).toInt(),commandValueString.toInt());

    // Button
    } else if (commandNameString.startsWith(CMD_BTN_SHORTPESS)) {
      Serial.println (F("Command RAISE BUTTON SHORT PRESS EVENT received"));
      EVENT_Button_Pressed_Short = true;
    } else if (commandNameString.startsWith(CMD_BTN_LONGPESS)) {
      Serial.println (F("Command RAISE BUTTON LONG PRESS EVENT received"));
      EVENT_Button_Pressed_Long= true;
    } else if (commandNameString.startsWith(CMD_BTN_VERYLONGPESS)) {
      Serial.println (F("Command RAISE BUTTON VERY LONG PRESS EVENT received"));
      EVENT_Button_Pressed_VeryLong= true;
    } else if (commandNameString.startsWith(CMD_BTN_CLEAR_STATUS)) {
      Serial.println (F("Command CLEAR BUTTON STATUS received"));
      Button_Status = 0;
    
    // Button events routines
    } else if (commandNameString.startsWith(CMD_EMERGENCY_HALT_FLAG_CLEAR)) {
      Serial.println (F("Command CLEAR EMERGENCY HALT CALLED FLAG received"));
      clearEmergencyHaltRoutine();
    } else if (commandNameString.startsWith(CMD_SOFTSTOP_FINISH_ROUTINE)) {
      Serial.println (F("Command FINISH SOFT SOFT ROUTINE received"));
      finishSoftStopRoutine();
    } else if (commandNameString.startsWith(CMD_SOFTSTOP_FLAGS_CLEAR)) {
      Serial.println (F("Command CLEAR SOFT STOP FLAGS received"));
      clearSoftStopFlags();

    // SHT Heater
    } else if (commandNameString.startsWith(CMD_SHT_HEATER_ON)) {
      Serial.println (F("Command SWITCH SHT_HEATER ON received"));
      SHT_Heater_ON();
    } else if (commandNameString.startsWith(CMD_SHT_HEATER_OFF)) {
      Serial.println (F("Command SWITCH SHT_HEATER OFF received"));
      SHT_Heater_OFF();
   
   // ESP Core commands
    } else if (commandNameString.startsWith(CMD_ESP_REBOOT)) {
      Serial.println (F("Command ESP REBOOT received"));
      ESP.restart();
    } else {
      Serial.print ("Unkown command <");
      Serial.print (commandNameString);
      Serial.println ("> received");
    }
}

/*
 *  Parse SERIAL command 
 *  prepare command from serial from parsing and pass it to command processing
 *  input string like "[?PM1=100]"
 */
void parseInputCommands(String serialString)
{
  String commandNameSt, commandValueSt;
  // check if this is a command
  if (serialString.startsWith(SERIAL_COMMAND_START) && serialString.endsWith(SERIAL_COMMAND_END))
  {
    // get command name
    String commandNameSt = serialString.substring(SERIAL_COMMAND_START_LENGTH, SERIAL_COMMAND_START_LENGTH+3);
    // get command value (if applicable)
    if (serialString.indexOf(SERIAL_COMMAND_VALUE_SIGN) >= 0) {
      commandValueSt = serialString.substring (serialString.indexOf(SERIAL_COMMAND_VALUE_SIGN)+1,serialString.length()-1);
    } else {
      commandValueSt = "";
    }
    
    #ifdef DEBUGF
      Serial.print("Serial command detected: '");
      Serial.print(commandNameSt);
      Serial.print("' with parameter '");
      Serial.print(commandValueSt);
      Serial.println("'");
    #endif

    parseCommand (commandNameSt, commandValueSt);
    
  } else {
  //this is not a command
    Serial.print("Unknown serial input <");
    Serial.print(serialString);
    Serial.println(">");
  }
}
