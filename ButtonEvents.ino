/* Button aux vars */
bool _prev_Button_Pressing                    = false;
bool _prev_Button_Pressing_Long               = false;
bool _prev_Button_Pressing_VeryLong           = false;
unsigned long _Button_pressed_time            = 0;

uint16_t Button_pushed_events_count           = 0;
uint16_t Button_all_events_count              = 0;

void Calculate_Button_Events()
{
      // this is pushed button event? (wasn't pressed before)
      if (EVENT_Button_Pressing && !_prev_Button_Pressing) {
          _prev_Button_Pressing = true;
          _Button_pressed_time = currentTime;
          EVENT_Button_Pressed_Short = false;
          EVENT_Button_Pressed_Long = false;
          EVENT_Button_Pressed_VeryLong = false;
          EVENT_Button_Pushed = true;
      
      // this is release button event? (was pressed before and now released)
      } else if (!EVENT_Button_Pressing && _prev_Button_Pressing) {
        
        // was pressed for VERY LONG?
        if ((currentTime - _Button_pressed_time) > BUTTON_VERY_LONG_PRESS_INTERVAL) {
          EVENT_Button_Pressed_VeryLong = true;
        // was pressed for LONG?
        } else if ((currentTime - _Button_pressed_time) > BUTTON_LONG_PRESS_INTERVAL) {
          EVENT_Button_Pressed_Long = true;
        // was pressed for SHORT
        } else {
          EVENT_Button_Pressed_Short = true;
        }
        _prev_Button_Pressing = false;
        _prev_Button_Pressing_Long = false;
        _prev_Button_Pressing_VeryLong = false;
        EVENT_Button_Released = true;                     // fire released event
        EVENT_Button_Pressing_Long_Reached = false;       // clear event on release
        EVENT_Button_Pressing_VeryLong_Reached = false;   // clear event on release
      
      // still pressing button?
      } else if (EVENT_Button_Pressing) {
        // fire pressing interval thresholds events (such as Pressing for a Very Long Period Reached)
        if ((currentTime - _Button_pressed_time) > BUTTON_VERY_LONG_PRESS_INTERVAL && !_prev_Button_Pressing_VeryLong) {
          EVENT_Button_Pressing_VeryLong_Reached = true;
          _prev_Button_Pressing_VeryLong = true;
        } else if ((currentTime - _Button_pressed_time) > BUTTON_LONG_PRESS_INTERVAL  && !_prev_Button_Pressing_Long) {
          EVENT_Button_Pressing_Long_Reached = true;
          _prev_Button_Pressing_Long = true;
        }
      }
}

/*
 * Handle button events
 * 
 * Events can be received through button pushing (hardware), or through commands from Serial/WEB
 * WARNING! Need to run Clear_Events() to clear events, or events would be raised every cycle
 * 
 * EVENT_Button_Pressing                    - still pressing the button. Not used
 * EVENT_Button_Pushed                      - just pushed; switch button led on and stop led ticker (used to blink button led)
 * EVENT_Button_Released                    - just relased; switch button led off and stop led ticker (used to blink button led)
 * EVENT_Button_Pressing_Long_Reached       - just Long Pressing (while holding) detected; start blinking BUTTON LED ((mode 1), switch on and start MAIN LED ticker ((mode 1)
 * EVENT_Button_Pressing_VeryLong_Reached   - just Very Long Pressing (while holding) detected; start blinking BUTTON LED ((mode 2)), switch on and start MAIN LED ticker (mode 2)
 * EVENT_Button_Pressed_Short               - switch off Main Led and cancel its ticker. set Button_Status = 1. stop EMERGENCY countdown
 * EVENT_Button_Pressed_Long                - switch on Main Led and stats its ticker (MODE 1). set Button_Status = 2
 * EVENT_Button_Pressed_VeryLong            - switch on Main Led and stats its ticker (MODE 2). set Button_Status = 3. Starts EMERGENCY countdown
 * 
 * 
 */
void Handle_Button_Events()
{
  /* Button was just pushed */
  if (EVENT_Button_Pushed) {
    ButtonLedTicker.detach();
    Button_Led_ON();
  }
  /* Button was just released */
  if (EVENT_Button_Released) {
    ButtonLedTicker.detach();
    Button_Led_OFF();
  }
  /* Button was pushed for long */
  if (EVENT_Button_Pressing_Long_Reached) {
    ButtonLedTicker.attach_ms(700, Button_Led_Blink);
    MainLed_SetColor(Yellow);
    MainLed_ON();
    MainLedTicker.attach_ms(300, MainLed_Blink);
  }
  /* Button was pushed for very long */
  if (EVENT_Button_Pressing_VeryLong_Reached) {
    ButtonLedTicker.attach_ms(200, Button_Led_Blink);
    MainLed_SetColor(Red);
    MainLed_ON();
    MainLedTicker.attach_ms(300, MainLed_Blink);
  }


  if (EVENT_Button_Pressed_Short) {
    Button_Status = 1;
    MainLed_OFF();
    MainLedTicker.detach();
    // Cancel EmergencyHalt Routine
    EmergencyHaltTicker.detach();
    // Cancel Soft Stop Routine
    SoftStopStartTicker.detach();
    // Clear Soft Stop flags
    clearSoftStopFlags();
  };
  if (EVENT_Button_Pressed_Long) {
    Button_Status = 2;
    MainLed_SetColor(Yellow);
    MainLed_ON();
    MainLedTicker.attach_ms(300, MainLed_Blink);
    SoftStopStartTicker.attach_ms(POWER_HALT_SOFTSTOP_START_DELAY, startSoftStopRoutine);
    // Run JustInCase Halt timer
    EmergencyHaltTicker.attach_ms(POWER_HALT_SOFTSTOP_FULL_HALT_DELAY, startEmergencyHaltRoutine);
  };
  if (EVENT_Button_Pressed_VeryLong) {
    Button_Status = 3;
    MainLed_SetColor(Red);
    MainLed_ON();
    MainLedTicker.attach_ms(300, MainLed_Blink);
    // Run Emergency Halt timer
    EmergencyHaltTicker.attach_ms(POWER_HALT_EMERGENCY_START_DELAY, startEmergencyHaltRoutine);
  }
}


void Clear_Events()
{
  /* Button events */
  EVENT_Button_Pressing = false;
  EVENT_Button_Pressing_Long_Reached  = false;
  EVENT_Button_Pressing_VeryLong_Reached  = false;
  EVENT_Button_Pushed = false;
  EVENT_Button_Released = false;
  EVENT_Button_Pressed_Short = false;
  EVENT_Button_Pressed_Long = false; 
  EVENT_Button_Pressed_VeryLong = false;
}

/*****************************************************************************************************************/

/*
 * Get button status (current)
 */
uint8_t readButton_current() {
  return mcp2.digitalRead(BUTTON_PIN);
}

/*
 * Async button status. Init procedure
 */
void readButton_init()
{
  Button_pushed_events_count = 0;
  Button_all_events_count = 0;
  _lastGetResult_BUTTON = millis();
}

/*
 * Async button status. Intermediate read
 */
void readButton_async()
{
  // test read to insure mcp is present
  uint8_t readTestPin = mcp2.digitalRead(MCP2_TEST_PIN); 
  if (readTestPin == HIGH && checkStatusMCP2 == 0)
  {
    uint8_t readValue = mcp2.digitalRead(BUTTON_PIN); 
    if (readValue == HIGH) 
    {
      Button_pushed_events_count++;
    }
    Button_all_events_count++;
  }
}

/*
 * Async button status. Calc result after series of intermediate reads
 */
uint8_t readButton_result()
{
  uint8_t pushed = 0;
  
  // check to skip Divide by Zero exception
  if (Button_pushed_events_count > 0) {
  
    float percent = (float) Button_pushed_events_count/ (float) Button_all_events_count * 100.0;
    pushed = (percent > 5.0 ? 1 : 0);
  
    #ifdef DEBUGF
      Serial.print("Pushed events= ");
      Serial.print(Button_pushed_events_count);
      Serial.print(" ");
      Serial.print("of total= ");
      Serial.print(Button_all_events_count);
      Serial.print(", ");
      Serial.print(percent);
      Serial.print("% ");
      Serial.print("Result: ");
      Serial.print(pushed);
      Serial.println();
    #endif
  }
  
  // reinit all values
  readButton_init();
  
  return pushed;
}


/*
 * Switch button led ON
 */
void Button_Led_ON() {
  mcp2.digitalWrite(BUTTON_LED_PIN, HIGH);
}

/*
 * Switch button led OFF
 */
void Button_Led_OFF() {
  mcp2.digitalWrite(BUTTON_LED_PIN, LOW);
}

/*
 * Get button led status
 */
uint8_t Button_Led_Status() {
  return mcp2.digitalRead(BUTTON_LED_PIN);
}

/*
 * Switch button led ON
 */
void Button_Led_Blink() {
  if (Button_Led_Status())
    Button_Led_OFF();
  else
    Button_Led_ON();
}
