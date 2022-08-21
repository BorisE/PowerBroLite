#define SENSOR_RELAY_STATUS(s)              STR(RL##s)    // effectively results to "RL1"
#define SENSOR_PWM(s)                       STR(PM##s)    // effectively results to "PM1"

#define SENSOR_INA_VOLTAGE_PREFIX           "VL"          // should be postfixed with number, e.g. VL1
#define SENSOR_INA_CURRENT_PREFIX           "CR"
#define SENSOR_INA_POWER_PREFIX             "PW"
#define SENSOR_INA_VOLTAGE(s)               STR(VL##s)
#define SENSOR_INA_CURRENT(s)               STR(CR##s)
#define SENSOR_INA_POWER(s)                 STR(PW##s)

#define SENSOR_BUTTON_STATUS                "BTN"
#define SENSOR_EMERGENCY_HALT_FIRED         "EHF"
#define SENSOR_SOFTSTOP_NEED_TO_START       "SSR"         //SoftStop_NeedToStart
#define SENSOR_SOFTSTOP_FIRED               "SSF"         //SoftStop_WasRun
#define SENSOR_LED_STATUS                   "LED"
#define SENSOR_LED_COLOR                    "LEC"

#define SENSOR_ESP_FREE_MEMORY              "MEM"         
#define SENSOR_ESP_RUNTIME                  "RTM"         

#define SENSOR_FIRMWARE_VER                 "FWV"         
#define SENSOR_FIRMWARE_DATE                "FWD"         

#define SENSOR_NETWORK_CONNECTION_STATUS    "NTW"         //WiFi.Status()
#define SENSOR_NETWORK_WIFI_SSID            "NTS"         //WiFi.SSID()
#define SENSOR_NETWORK_WIFI_RSSI            "NTR"         //WiFi.RSSI()
#define SENSOR_NETWORK_WIFI_BSSID           "NTB"         //WiFi.BSSIDstr()
#define SENSOR_NETWORK_WIFI_CHANNEL         "NTC"         //WiFi.channel()
#define SENSOR_NETWORK_MAC_ADDRESS          "NTM"         //WiFi.macAddress()
#define SENSOR_NETWORK_IP_ADDRESS           "NTA"         //WiFi.localIP()

#define SENSOR_ERROR_STATUS                 "ERR"         //hardwareERRstatus
#define SENSOR_ERROR_DETAILS_STRING         "ERS"         //hardwareERRstring

#define SENSOR_SHT_TEMPERATURE              "STM"         // SHT temperature value
#define SENSOR_SHT_HUMIDITY                 "STH"         // SHT humidity value
#define SENSOR_SHT_HEATER_STATUS            "SHE"         // SHT heater status

#define SENSOR_MLX_OBJECT                   "MOB"         // MLX90614 object temperature value
#define SENSOR_MLX_AMBIENT                  "MAM"         // MLX90614 ambient temperature value
#define SENSOR_MLX_CLOUD                    "MCL"         // MLX90614 ambient temperature value

void SensorsSerial_Often() 
{
 
  //Relay 1
  Serial.print(SENSOR_OUT_START);
  Serial.print(SENSOR_RELAY_STATUS(1) ":");
  Serial.print(Relay1StatusFlags, BIN);
  Serial.println(SENSOR_OUT_END);

  //Relay 2
  Serial.print(SENSOR_OUT_START);
  Serial.print(SENSOR_RELAY_STATUS(2) ":");
  Serial.print(Relay2StatusFlags, BIN);
  Serial.println(SENSOR_OUT_END);

  //PWM1
  Serial.print(SENSOR_OUT_START);
  Serial.print(SENSOR_PWM(1) ":");
  Serial.print(PWM1_Level);
  Serial.println(SENSOR_OUT_END);

  //PWM2
  Serial.print(SENSOR_OUT_START);
  Serial.print(SENSOR_PWM(2) ":");
  Serial.print(PWM2_Level);
  Serial.println(SENSOR_OUT_END);

  //PWM3
  Serial.print(SENSOR_OUT_START);
  Serial.print(SENSOR_PWM(3) ":");
  Serial.print(PWM3_Level);
  Serial.println(SENSOR_OUT_END);

  //INA
  for(int i=0; i<INA_COUNT; i++)
  {
      Serial.print(SENSOR_OUT_START);
      Serial.print(SENSOR_INA_VOLTAGE_PREFIX);
      Serial.print(i);
      Serial.print(":");
      Serial.print(ina_volt[i],3);
      Serial.println(SENSOR_OUT_END);
    
      Serial.print(SENSOR_OUT_START);
      Serial.print(SENSOR_INA_CURRENT_PREFIX);
      Serial.print(i);
      Serial.print(":");
      Serial.print(ina_cur[i],3);
      Serial.println(SENSOR_OUT_END);
    
      Serial.print(SENSOR_OUT_START);
      Serial.print(SENSOR_INA_POWER_PREFIX);
      Serial.print(i);
      Serial.print(":");
      Serial.print(ina_pwr[i],1);
      Serial.println(SENSOR_OUT_END);
  }

  //BTN
  Serial.print(SENSOR_OUT_START);
  Serial.print(SENSOR_BUTTON_STATUS ":");
  Serial.print(Button_Status);
  Serial.println(SENSOR_OUT_END);

  //LED
  Serial.print(SENSOR_OUT_START);
  Serial.print(SENSOR_LED_STATUS ":");
  Serial.print(MainLed_Status);
  Serial.println(SENSOR_OUT_END);
  Serial.print(SENSOR_OUT_START);
  Serial.print(SENSOR_LED_COLOR ":");
  Serial.print(MainLed_Color);
  Serial.println(SENSOR_OUT_END);

  //SHT
  #ifdef ENABLE_SHT31
    Serial.print(SENSOR_OUT_START);
    Serial.print(SENSOR_SHT_TEMPERATURE ":");
    Serial.print(SHT_T);
    Serial.println(SENSOR_OUT_END);
    Serial.print(SENSOR_OUT_START);
    Serial.print(SENSOR_SHT_HUMIDITY ":");
    Serial.print(SHT_H);
    Serial.println(SENSOR_OUT_END);
    Serial.print(SENSOR_OUT_START);
    Serial.print(SENSOR_SHT_HEATER_STATUS ":");
    Serial.print(SHT_HeaterStatus);
    Serial.println(SENSOR_OUT_END);
  #endif

  // MLX
  #ifdef ENABLE_MLX90614  
    Serial.print(SENSOR_OUT_START);
    Serial.print(SENSOR_MLX_OBJECT ":");
    Serial.print(MLX_obj);
    Serial.println(SENSOR_OUT_END);
    Serial.print(SENSOR_OUT_START);
    Serial.print(SENSOR_MLX_AMBIENT ":");
    Serial.print(MLX_amb);
    Serial.println(SENSOR_OUT_END);
    Serial.print(SENSOR_OUT_START);
    Serial.print(SENSOR_MLX_CLOUD ":");
    Serial.print(MLX_cloud);
    Serial.println(SENSOR_OUT_END);
  #endif
 
  if (EmergencyHalt_Fired > 0) {
    Serial.print(SENSOR_OUT_START);
    Serial.print(SENSOR_EMERGENCY_HALT_FIRED ":");
    Serial.print(EmergencyHalt_Fired);
    Serial.println(SENSOR_OUT_END);
  }
  if (SoftStop_NeedToStart > 0) {
    Serial.print(SENSOR_OUT_START);
    Serial.print(SENSOR_SOFTSTOP_NEED_TO_START ":");
    Serial.print(SoftStop_NeedToStart);
    Serial.println(SENSOR_OUT_END);
  }
  if (SoftStop_WasRun > 0) {
    Serial.print(SENSOR_OUT_START);
    Serial.print(SENSOR_SOFTSTOP_FIRED ":");
    Serial.print(SoftStop_WasRun);
    Serial.println(SENSOR_OUT_END);
  }

  Serial.print(SENSOR_OUT_START);
  Serial.print(SENSOR_ESP_RUNTIME ":");
  Serial.print(currentTime);
  Serial.println(SENSOR_OUT_END);

  #ifdef DEBUGF
    Serial.println();
  #endif
}

void SensorsSerial_Rare() 
{

  Serial.print(SENSOR_OUT_START);
  Serial.print(SENSOR_ESP_FREE_MEMORY ":");
  Serial.print(ESP.getFreeHeap());
  Serial.println(SENSOR_OUT_END);
  

  Serial.print(SENSOR_OUT_START);
  Serial.print(SENSOR_FIRMWARE_VER ":");
  Serial.print(VERSION);
  #ifdef DEBUGF
  Serial.print("-debug");
  #endif
  #ifdef DEBUGF_MORE
  Serial.print("-more");
  #endif
  Serial.println(SENSOR_OUT_END);

  Serial.print(SENSOR_OUT_START);
  Serial.print(SENSOR_FIRMWARE_DATE ":");
  Serial.print(VERSION_DATE);
  Serial.println(SENSOR_OUT_END);

  Serial.print(SENSOR_OUT_START);
  Serial.print(SENSOR_NETWORK_CONNECTION_STATUS ":");
  Serial.print(WiFi.status());        //wifi connection status
  Serial.println(SENSOR_OUT_END);

  if (WiFi.status() == WL_CONNECTED) {
    iotWebConf.blink(5000,100);
    Serial.print(SENSOR_OUT_START);
    Serial.print(SENSOR_NETWORK_WIFI_SSID ":");
    Serial.print(WiFi.SSID());        //SSID
    Serial.println(SENSOR_OUT_END);

    Serial.print(SENSOR_OUT_START);
    Serial.print(SENSOR_NETWORK_WIFI_RSSI ":");
    Serial.print(WiFi.RSSI());        // RSSI (signal strength)
    Serial.println(SENSOR_OUT_END);

    Serial.print(SENSOR_OUT_START);
    Serial.print(SENSOR_NETWORK_WIFI_BSSID ":");             
    Serial.print(WiFi.BSSIDstr());    // mac of ssid?
    Serial.println(SENSOR_OUT_END);

    Serial.print(SENSOR_OUT_START);
    Serial.print(SENSOR_NETWORK_WIFI_CHANNEL ":");
    Serial.print(WiFi.channel());     //WiFi channel
    Serial.println(SENSOR_OUT_END);
    
    Serial.print(SENSOR_OUT_START);
    Serial.print(SENSOR_NETWORK_MAC_ADDRESS ":");
    Serial.print(WiFi.macAddress());  //MAC Address
    Serial.println(SENSOR_OUT_END);

    Serial.print(SENSOR_OUT_START);
    Serial.print(SENSOR_NETWORK_IP_ADDRESS ":");
    Serial.print(WiFi.localIP());     //Local IP
    Serial.println(SENSOR_OUT_END);

    #ifdef DEBUGF
      Serial.println();
    #endif
  }

}

String SensorsJSON()
{
  String page = "{";

  for(int i=1; i<=6; i++) {
    page += "\"" SENSOR_RELAY_STATUS(1) + String(i) + "\": " + String(getRelayStatus(1,i)) + ",";
    page += "\"" SENSOR_RELAY_STATUS(2) + String(i) + "\": " + String(getRelayStatus(2,i)) + ",";
  }
  page += "\"" SENSOR_PWM(1) "\": " + String(PWM1_Level) + ",";
  page += "\"" SENSOR_PWM(2) "\": " + String(PWM2_Level) + ",";
  page += "\"" SENSOR_PWM(3) "\": " + String(PWM3_Level) + ",";

  for(int i=0; i<INA_COUNT; i++) {
      page += "\"" SENSOR_INA_VOLTAGE_PREFIX + String(i) + "\": \"" + String(ina_volt[i],3) + "\",";
      page += "\"" SENSOR_INA_CURRENT_PREFIX + String(i) + "\": \"" + String(ina_cur[i],3) + "\",";
      page += "\"" SENSOR_INA_POWER_PREFIX + String(i) + "\": \"" + String(ina_pwr[i],3) + "\",";
  }

  page += "\"" SENSOR_BUTTON_STATUS "\": "          + String(Button_Status) + ",";

  page += "\"" SENSOR_LED_STATUS "\": "             + String(MainLed_Status) + ",";
  page += "\"" SENSOR_LED_COLOR "\": "              + String(MainLed_Color) + ",";

  #ifdef ENABLE_SHT31
  page += "\"" SENSOR_SHT_TEMPERATURE "\": \""      + String(SHT_T, 1) + "\",";
  page += "\"" SENSOR_SHT_HUMIDITY "\": \""         + String(SHT_H, 1) + "\",";
  page += "\"" SENSOR_SHT_HEATER_STATUS "\": \""    + String(SHT_HeaterStatus) + "\",";
  #endif

  #ifdef ENABLE_MLX90614
  page += "\"" SENSOR_MLX_OBJECT "\": \""           + String(MLX_obj, 1) + "\",";
  page += "\"" SENSOR_MLX_AMBIENT "\": \""          + String(MLX_amb, 1) + "\",";
  page += "\"" SENSOR_MLX_CLOUD "\": \""            + String(MLX_cloud, 1) + "\",";  
  #endif
  
  page += "\"" SENSOR_EMERGENCY_HALT_FIRED "\": "   + String(EmergencyHalt_Fired) + ",";
  page += "\"" SENSOR_SOFTSTOP_NEED_TO_START "\": " + String(SoftStop_NeedToStart) + ",";
  page += "\"" SENSOR_SOFTSTOP_FIRED "\": "         + String(SoftStop_WasRun) + ",";

  page += "\"" SENSOR_ERROR_STATUS "\": "           + String(hardwareERRstatus) + ",";
  page += "\"" SENSOR_ERROR_DETAILS_STRING "\": \"" + String(hardwareERRstring) + "\",";

  page += "\"" SENSOR_ESP_FREE_MEMORY "\": "        + String(ESP.getFreeHeap()) + ",";

  if (WiFi.status() == WL_CONNECTED) {
    page += "\"" SENSOR_NETWORK_WIFI_SSID "\": \""  + String(WiFi.SSID()) + "\",";
    page += "\"" SENSOR_NETWORK_WIFI_RSSI "\": \""  + String(WiFi.RSSI()) + "\",";
    page += "\"" SENSOR_NETWORK_WIFI_BSSID "\": \"" + String(WiFi.BSSIDstr()) + "\",";
    page += "\"" SENSOR_NETWORK_WIFI_CHANNEL "\": \"" + String(WiFi.channel()) + "\",";
    page += "\"" SENSOR_NETWORK_IP_ADDRESS "\": \"" + String(WiFi.localIP().toString()) + "\",";
  } else {
    page += "\"" SENSOR_NETWORK_WIFI_SSID "\": \"N/A\",";
    page += "\"" SENSOR_NETWORK_WIFI_RSSI "\": \"N/A\",";
    page += "\"" SENSOR_NETWORK_WIFI_BSSID "\": \"N/A\",";
    page += "\"" SENSOR_NETWORK_WIFI_CHANNEL "\": \"N/A\",";
    page += "\"" SENSOR_NETWORK_IP_ADDRESS "\": \"N/A\",";
  }
  page += "\"" SENSOR_NETWORK_MAC_ADDRESS "\": \"" + String(WiFi.macAddress()) + "\",";

  page += "\"debug\": \"" + String(debugstack) + "\",";
  page += "\"" SENSOR_FIRMWARE_VER "\": \"" + String(VERSION) + "\", ";
  page += "\"" SENSOR_FIRMWARE_DATE "\": \"" + String(VERSION_DATE) + "\",";

  page += "\"" SENSOR_ESP_RUNTIME "\": \"" + formatTimeElapsed(currentTime) + "\"";
  page +="}";

  debugstack[0] = '\0'; //empty buffer

  return page;
}
