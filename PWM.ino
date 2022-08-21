void setPWM (uint8_t PWM_Out, uint16_t PWM_Level)
{
  PWM_Level = constrain (PWM_Level, 0, 1023);
  if (PWM_Out == 1) {
    analogWrite (PWM1_PIN, PWM_Level);
    PWM1_Level = PWM_Level;
  }
  else if (PWM_Out == 2) {
    analogWrite (PWM2_PIN, PWM_Level);
    PWM2_Level = PWM_Level;
  }
  else if (PWM_Out == 3) {
    analogWrite (PWM3_PIN, PWM_Level);
    PWM3_Level = PWM_Level;
  }
  else {
    #ifdef DEBUGF
      Serial.print("Couldn't found PWM");
      Serial.print(PWM_Out);
      Serial.println();
    #endif
  }
  #ifdef DEBUGF
    Serial.print("Set PWM");
    Serial.print(PWM_Out);
    Serial.print(" to ");
    Serial.println(PWM_Level);
  #endif
}
