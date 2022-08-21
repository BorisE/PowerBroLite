/*
 * [get] Function to return relay status in bool format
 * returns flase (OFF, OPENED) or true (ON, CLOSED) state
 * can be used directly as opposed to setRelayStatus
 */
bool getRelayStatus(uint8_t relayBlock, uint8_t relay) 
{
  switch (relay) {
    case 1: 
      return (mcp1.digitalRead(RELAY1_CH1_PIN) == RELAY1_ON_STATE[relay]);
      break;
    case 2: 
      return (mcp1.digitalRead(RELAY1_CH2_PIN) == RELAY1_ON_STATE[relay]);
      break;
    case 3: 
      return (mcp1.digitalRead(RELAY1_CH3_PIN) == RELAY1_ON_STATE[relay]);
      break;
    case 4: 
      return (mcp1.digitalRead(RELAY1_CH4_PIN) == RELAY1_ON_STATE[relay]);
      break;
    case 5: 
      return (mcp1.digitalRead(RELAY1_CH5_PIN) == RELAY1_ON_STATE[relay]);
      break;
    case 6: 
      return (mcp1.digitalRead(RELAY1_CH6_PIN) == RELAY1_ON_STATE[relay]);
      break;
    default:
      #ifdef DEBUGF
        Serial.print("Relay not found! getRelayStatus. RelayBlock <");
        Serial.print(relayBlock);
        Serial.print(">. Get relay [");
        Serial.print(relay);
        Serial.print("]");
        Serial.println();
      #endif
      return LOW;  
  }
}

/* 
 * [set] Change relay status
 * normaly don't call it directly, use one of the wrappers
 * stat - raw relay state (HIGH or LOW), not as OPEN or CLOSE
 */
void setRelaySwitch(uint8_t relayBlock, uint8_t relay, uint8_t stat)
{
  #ifdef DEBUGF
    Serial.print("setRelaySwitch. RelayBlock <");
    Serial.print(relayBlock);
    Serial.print(">. Set [");
    Serial.print(relay);
    Serial.print("]=");
    Serial.print(stat);
    Serial.println();
  #endif
  switch (relay) {
    case 1: 
      return mcp1.digitalWrite(RELAY1_CH1_PIN, stat);
      break;
    case 2: 
      return mcp1.digitalWrite(RELAY1_CH2_PIN, stat);
      break;
    case 3: 
      return mcp1.digitalWrite(RELAY1_CH3_PIN, stat);
      break;
    case 4: 
      return mcp1.digitalWrite(RELAY1_CH4_PIN, stat);
      break;
    case 5: 
      return mcp1.digitalWrite(RELAY1_CH5_PIN, stat);
      break;
    case 6: 
      return mcp1.digitalWrite(RELAY1_CH6_PIN, stat);
      break;
  }
}


/*
 * [set] Function to switch relay based on char type status
 */
void setRelaySwitch_Str(uint8_t relayBlock, uint8_t relay, const char* relayTargetStat )
{
  /*Serial.print("relayTargetStat=");
  Serial.println(relayTargetStat);*/
  
  if (strcmp(relayTargetStat, "1") == 0) {
    RelayOn(relayBlock, relay);
  } else if (strcmp(relayTargetStat, "0") == 0) {
    RelayOff(relayBlock, relay);
  }
}

/*
 * [set] Wrapper to Set Relay ON
 */
void RelayOn(uint8_t relayBlock, uint8_t relay) 
{
  setRelaySwitch(relayBlock, relay, (relayBlock == 1 ? RELAY1_ON_STATE[relay] : RELAY2_ON_STATE[relay]));
}

/*
 * [set] Wrapper to Set Relay OFF
 */
void RelayOff(uint8_t relayBlock, uint8_t relay) 
{
  setRelaySwitch(relayBlock, relay, !(relayBlock == 1 ? RELAY1_ON_STATE[relay] : RELAY2_ON_STATE[relay]));
}


/*
 * Returns RELAY 1 Status for each channel in one string
 * 1 - ON (CLOSED), 0 - OFF (OPENED)
 */
uint8_t Relay1StatusAll()
{
    uint8_t ret = 0;
    for (int i=1; i<=6; i++)
       bitWrite(ret, i-1, getRelayStatus(1, i)); // starts from least significant, ie. rightmost, ie. RL 1 = 0b00000001, RL 2 = 0b00000010, etc)
    
    return ret;
}
/**************************************************
 * Function used to override software switching off
 * Fired upon Very Long button pressed
 * 
 * After completing its routine, starts sending emergency halt flag to output. Should be manually (externaly) cleared
 **************************************************/
void startEmergencyHaltRoutine()
{
    // stop recurring running of the routine
    EmergencyHaltTicker.detach(); 

    Serial.println(F("Emergency halt signaled. Starting routine"));

    // clear Need to Run SoftStop and SoftStop was run - it becomes now fully useless
    SoftStopStartTicker.detach(); 
    SoftStop_NeedToStart = 0;
    SoftStop_WasRun = 0;

    //********//
    // Switch off all relays
    RelayOff(1,1);
    RelayOff(1,2);
    RelayOff(1,3);
    RelayOff(1,4);
    RelayOff(1,5);
    RelayOff(1,6);

    MainLedTicker.detach();
    MainLed_SetColor(Red);
    MainLed_ON();
    EmergencyHalt_Fired = 1;
}
/**************************************************
 * Function used to clear markers of EmergencyHalt routine (LED and EmergencyHalt_Fired)
 * Can be used after routines was run (primary use) or even before
 **************************************************/
void clearEmergencyHaltRoutine()
{
    // stop Emergency Halt timer
    EmergencyHaltTicker.detach(); 

    #ifdef DEBUGF
    Serial.println(F("Clearing Emergency Halt routines flags"));
    #endif

    // Clear flags
    EmergencyHalt_Fired = 0;

    // Switch off leds
    MainLedTicker.detach();
    MainLed_OFF();
}

/**************************************************
 * Function used to start software stop routine
 * Fired upon Long button pressed (with POWER_HALT_SOFTSTOP_START_DELAY delay)
 * 
 * WARNING!!! After (externaly) completing routine, endSoftStopRoutine() should be called to prevent
 * runing EmergencyHalt routine
 **************************************************/
void startSoftStopRoutine()
{
    // stop recurring running of the routine
    SoftStopStartTicker.detach(); 

    Serial.println(F("Soft stop signaled. Starting routine"));

    // Set SoftStop flags
    SoftStop_NeedToStart = 1;
    SoftStop_WasRun = 0;
    // Clear EmergencyHalt flag (erase memory of beeing run)
    EmergencyHalt_Fired = 0;
    
    MainLed_SetColor(Yellow);
    MainLedTicker.attach_ms(1000, MainLed_Blink);
}

/**************************************************
 * Function used to mark end (externaly) of SoftStop routine.
 * Need to be called (externally) to prevent running EmergencyHalt routine
 **************************************************/
void finishSoftStopRoutine()
{
    // stop Emergency Halt timer
    EmergencyHaltTicker.detach(); 
    // just in case stop recurring running of the routine
    SoftStopStartTicker.detach(); 

    #ifdef DEBUGF
    Serial.println(F("SoftStop routine was finished"));
    #endif

    // Clear flags
    SoftStop_NeedToStart = 0;
    SoftStop_WasRun = 1;

    // Switch led to indicate that routine was finished
    MainLedTicker.detach();
    MainLed_SetColor(Yellow);
    MainLed_ON();
}
/**************************************************
 * Function used to clear markers of SoftStop routine runing (LED and SoftStop_WasRun)
 * Can be used after routines was run after some time
 **************************************************/
void clearSoftStopFlags()
{
    // stop Emergency Halt timer
    EmergencyHaltTicker.detach(); 
    // just in case stop recurring running of the routine
    SoftStopStartTicker.detach(); 

    #ifdef DEBUGF
    Serial.println(F("Clearing SoftStop flags"));
    #endif

    // Clear flags
    SoftStop_NeedToStart = 0;
    SoftStop_WasRun = 0;

    // Switch off leds
    MainLedTicker.detach();
    MainLed_OFF();
}
