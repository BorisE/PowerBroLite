/*
 * Parse command for Main Led switching
 * SwitchState = 1|0
 */
void MainLed_SwitchCommand( uint8_t SwitchState ) 
{
  if (SwitchState)
    MainLed_ON();
  else 
    MainLed_OFF();
}

/*
 * Parse command for Main Led color switching. 
 * 
 * if led was switched off, it would be turned on.
 * if targetColor = Led_Off, led would be switched off
 */
void MainLed_ColorCommand( int targetColor ) 
{
  if ((Colors_Enum)targetColor == Led_Off)
    MainLed_OFF();
  else 
  {
    MainLed_SetColor ((Colors_Enum)targetColor);
    MainLed_ON();
  }
}


/*
 * Physically switch LED ON based on previously set color - MainLed_SetColor()
 */
void MainLed_ON() 
{
  MainLed_Status = HIGH;

  // if no color was set, set it to white
  if (MainLed_Color ==  Led_Off)
    MainLed_SetColor (White);
  else
    MainLed_SetColor(MainLed_Color);

  mcp1.digitalWrite(MAINLED_R_PIN, MainLed_RedStatus);
  mcp1.digitalWrite(MAINLED_G_PIN, MainLed_GreenStatus);
  mcp1.digitalWrite(MAINLED_B_PIN, MainLed_BlueStatus);
}

/*
 * Switch Color Led OFF
 */
void MainLed_OFF() 
{
  MainLed_Status = LOW;

  mcp1.digitalWrite(MAINLED_R_PIN, LOW);
  mcp1.digitalWrite(MAINLED_G_PIN, LOW);
  mcp1.digitalWrite(MAINLED_B_PIN, LOW);
}

void MainLed_Blink() 
{
  if (MainLed_Status)
    MainLed_OFF();
  else 
    MainLed_ON();
}

/* 
 * Function to set color 
 * Then you can call MainLed ON to switch this color on 
 * Use global var MainLed_Color to get current color
 */
void MainLed_SetColor (Colors_Enum setColor) 
{
  if (setColor == Led_Off) 
    return;
  
  MainLed_Color = setColor;
  
  if (setColor == Red || setColor == Yellow || setColor == Magenta || setColor == White)
    MainLed_RedStatus = HIGH;
  else 
    MainLed_RedStatus = LOW;

  if (setColor == Green || setColor == Yellow || setColor == Cyan || setColor == White)
    MainLed_GreenStatus = HIGH;
  else 
    MainLed_GreenStatus = LOW;

  if (setColor == Blue || setColor == Magenta || setColor == Cyan || setColor == White)
    MainLed_BlueStatus = HIGH;
  else 
    MainLed_BlueStatus = LOW;
}

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
