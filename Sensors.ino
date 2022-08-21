#ifdef ENABLE_SHT31

void ReadSHT() 
{
    float t = sht31.readTemperature();
    float h = sht31.readHumidity();      
    
    if (! isnan(t)) {  // check if 'is not a number'
      SHT_T = t;
      SHT_ReadErrorsCount = 0;
      //Serial.print("Temp *C = "); Serial.print(t); Serial.print("\t\t");
    } else { 
      SHT_ReadErrorsCount++;
      //Serial.println("Failed to read temperature");
    }
    
    if (! isnan(h)) {  // check if 'is not a number'
      SHT_H = h;
      SHT_ReadErrorsCount = 0;
      //Serial.print("Hum. % = "); Serial.println(h);
    } else { 
      //Serial.println("Failed to read humidity");
    }
    #ifdef DEBUGF
      Serial.print(F("SHT errors count: "));
      Serial.println(SHT_ReadErrorsCount);
    #endif

    SHT_RunHeater();
}


void SHT_RunHeater()
{
  /* 1. May be we need to switch heater on? */
  // May be we doesn't run heater for too long?
  if (currentTime - _lastSHTHeaterOFF > SHT_HEATER_MAXIMUM_OFF_TIME_INTERVAL) {
    SHT_Heater_need_to_switch_ON = 1;
    #ifdef DEBUGF
      Serial.println(F("SHT heater wasn't switched for too long. Lets switch it on!"));
    #endif
  } else if (SHT_H > SHT_HEATER_HUMIDITY_THRESHOLD_TO_SWITCH_ON) {
    SHT_Heater_need_to_switch_ON = 1;
    #ifdef DEBUGF
      Serial.println(F("SHT humidty is too high, switch heater on!"));
    #endif
  }

  /* 2. Process heater actions */
  if (SHT_Heater_need_to_switch_ON == 1 && ((currentTime - _lastSHTHeaterOFF > SHT_HEATER_MINIMUM_OFF_INTERVAL) || _lastSHTHeaterOFF == 0 )  && SHT_HeaterStatus != SHT_HEATER_ON) {
    SHT_Heater_ON();
  } else if (SHT_HeaterStatus == SHT_HEATER_ON) {
    if (currentTime - _lastSHTHeaterON > SHT_HEATER_KEEP_ON_INTERVAL)
      SHT_Heater_OFF();
  } else if (SHT_HeaterStatus == SHT_HEATER_COOLDONW) {
    if (currentTime - _lastSHTHeaterOFF > SHT_HEATER_WAIT_COOLDOWN_INTERVAL) {
      SHT_HeaterStatus = SHT_HEATER_OFF;
    }
  }
}

void SHT_Heater_ON()
{
  SHT_Heater_need_to_switch_ON = 0; // clear flag just in case
  
  // An ~3.0 degC temperature increase can be noted when heater is enabled
  sht31.heater(1);
  
  SHT_HeaterStatus = SHT_HEATER_ON;
  _lastSHTHeaterON = currentTime;
  #ifdef DEBUGF
    Serial.println(F("SHT heater switching on"));
  #endif
}
void SHT_Heater_OFF()
{
  // An ~3.0 degC temperature increase can be noted when heater is enabled
  sht31.heater(0);
  SHT_HeaterStatus = SHT_HEATER_COOLDONW;
  _lastSHTHeaterOFF = currentTime;
  #ifdef DEBUGF
    Serial.println(F("SHT heater switching off"));
  #endif
}

#endif



#ifdef ENABLE_MLX90614

void ReadMLX()
{
  MLX_obj = mlx.readObjectTempC();
  MLX_amb = mlx.readAmbientTempC();
  MLX_cloud = mlx.readObjectTempC() - (checkStatusSHT == 0 ? SHT_T : mlx.readAmbientTempC());
}

#endif
