uint16_t ina_countval[INA_COUNT];
float ina_volt_sum[INA_COUNT];
float ina_cur_sum[INA_COUNT];

/*
 * Init INA reatinds
 */
void readINA_init()
{
  for(int i=0; i<INA_COUNT; i++)
  {
    ina_volt_sum[i] = 0;
    ina_cur_sum[i] = 0;
    ina_countval[i] = 0;
  }
  
  _lastGetResult_INA = millis();
}

/*
 * Async read INA data. Calc result after series of intermediate reads in readINA_result()
 */
void readINA_async()
{
  for(int i=0; i<INA_COUNT; i++)
  {
    ina_volt_sum[i] += INA_OBJ[i].getVoltage();
    ina_cur_sum[i] += INA_OBJ[i].getCurrent();
    ina_countval[i]++;
  }
}

/*
 * Calc result data after series of intermediate reads
 */
void readINA_result()
{
  for(int i=0; i<INA_COUNT; i++)
  {
    if (ina_countval[i] > 0) {
    
      ina_volt[i] = ina_volt_sum[i] / ina_countval[i];
      ina_cur[i]  = ina_cur_sum[i] / ina_countval[i];
      ina_pwr[i]  = ina_volt[i] * ina_cur[i];
    
      #ifdef DEBUGF_MORE
        // Читаем напряжение
        Serial.print("INA[");
        Serial.print(i);
        Serial.print("]:\t\t");
        Serial.print(F("Voltage:\t"));
        Serial.print(ina_volt[i], 3);
        Serial.print(F(" V\t"));
      
        // Читаем ток
        Serial.print(F("Current:\t"));
        Serial.print(ina_cur[i], 3);
        Serial.print(F(" A\t"));
      
        Serial.print(F("Num readings:\t"));
        Serial.println(ina_countval[i]);
      #endif
    }
  }
    
  readINA_init(); // reset data
}
