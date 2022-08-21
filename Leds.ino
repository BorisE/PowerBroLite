/********************************************************
 * STATUS LED
 *********************************************************/
/*
 * Long Blink starting procedure
 */
void StatusLed_Long_Start()
{
  StatusLed_ON();
  StatusLedTicker.attach_ms(1000, StatusLed_Long_Start_OFF);
}
void StatusLed_Long_Start_OFF()
{
  StatusLedTicker.detach();
  StatusLed_OFF();
}


/*
 * Two short Blink final procedure
 */
void StatusLed_TwoShort_Start()
{
  StatusLed_ON();
  StatusLedTicker.attach_ms(500, StatusLed_TwoShort_OFF1);
}

void StatusLed_TwoShort_OFF1()
{
  StatusLed_OFF();
  StatusLedTicker.detach();
  StatusLedTicker.attach_ms(100, StatusLed_TwoShort_ON2);
}

void StatusLed_TwoShort_ON2()
{
  StatusLed_ON();
  StatusLedTicker.attach_ms(500, StatusLed_TwoShort_OFF2);
}

void StatusLed_TwoShort_OFF2()
{
  StatusLed_OFF();
  StatusLedTicker.detach();
}

/*
 * Simple Actions
 */
void StatusLed_ON() 
{
  digitalWrite(STATUS_LED, HIGH);
}

void StatusLed_OFF() 
{
  digitalWrite(STATUS_LED, LOW);
}
