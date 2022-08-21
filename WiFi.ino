bool WiFiConnectedRecent            = false;    // does WiFi was connected previously?  OBSOLETE?
uint16_t WiFiConnectionRetries      = 0;        // number of connection retries         OBSOLETE!

void Network_Connected()
{
    /* Override default iotWebConf blink behaviour when WiFi is connected (i want to be led on all the time)
     *  parameters:
     *   @repeatMs - Defines the the period of one on-off cycle in milliseconds. 
     *   @dutyCyclePercent - LED on/off percent. 100 means always on, 0 means always off. 
     */
    iotWebConf.blink(500,100); 

    Serial.println(F("==========================="));
    Serial.print(F("[NET] Connected to "));
    Serial.println(WiFi.SSID());
    Serial.print(F("[NET] IP address: "));
    Serial.println(WiFi.localIP());
    Serial.println(F("==========================="));

    dnsServer.stop();

    ////////////////////////////////
    // MDNS INIT
    if (MDNS.begin(host)) {
      MDNS.addService("http", "tcp", 80);
      Serial.print(F("[NET] Open http://"));
      Serial.print(host);
      Serial.println(F(".local to access PowerBox interface"));
    } else {
      Serial.println(F("[NET] Couldn't start MDNS server"));
    }

    // Update server
    httpUpdater.setup(&server, "/update");
    
    // Start web server
    server.begin();
    Serial.println(F("[NET] HTTP server started"));

    ////////////////////////////////
    // OTA Update
    ArduinoOTA.setHostname(host);
    ArduinoOTA.setPort(OTA_PORT);
    ArduinoOTA.onStart(onStartOTA);
    ArduinoOTA.onEnd([]() {
      Serial.println(F("\n[OTA] End"));
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("[OTA] Progress: %u%%\r", (progress / (total / 100)));
    });
    ArduinoOTA.onError(onErrorOTA);
    ArduinoOTA.begin();
    Serial.println(F("[OTA] OTA ready"));
    WiFiConnectedRecent = true;
}

/*
 * This function run on disconnect event
 * Because it runs every time ESP couldn't connect to WiFi (automatic reconnect feature), var WiFiDisconnectedRecent used to insure the routine will run only once
  */
void ____Network_disconnect() 
{
  if (WiFiConnectedRecent) { 
    iotWebConf.stopCustomBlink(); //returns to iotWebConf blink behaviour 
    Serial.println(F("[NET] WiFi disconnected"));
    Serial.println(F("[NET] Stoping all network services"));
    MDNS.end();
    server.stop();
    //ArduinoOTA.end(); //can't found method to stop this service
    Serial.println(F("[NET] But will try to reconnect again"));
    
    WiFiConnectedRecent = false;
    WiFiConnectionRetries = 0;
  }
}

void ____WiFiLed_Blink() 
{
  if (digitalRead(STATUS_LED) == HIGH) {
    digitalWrite(STATUS_LED, LOW);  //on
    //Serial.print(".");
  } else {
    digitalWrite(STATUS_LED, HIGH); //off
  }
}




// Сообщает статус wifi соединения
void _____WiFiEvent(WiFiEvent_t event) {
  Serial.print("EVENT CODE: ");
  Serial.println(event);

  switch (event) {
    case WIFI_EVENT_STAMODE_CONNECTED:
      Serial.println("WIFI_EVENT_STAMODE_CONNECTED");
      break;
    case WIFI_EVENT_STAMODE_AUTHMODE_CHANGE:
      Serial.println("WIFI_EVENT_STAMODE_AUTHMODE_CHANGE");
      break;
    case WIFI_EVENT_STAMODE_GOT_IP:
      Serial.println("WIFI_EVENT_STAMODE_GOT_IP");
      Serial.println(WiFi.localIP().toString());
      break;
    case WIFI_EVENT_STAMODE_DISCONNECTED:
      Serial.println("WIFI_EVENT_STAMODE_DISCONNECTED");
      break;
    case WIFI_EVENT_SOFTAPMODE_PROBEREQRECVED:
     Serial.println("WIFI_EVENT_SOFTAPMODE_PROBEREQRECVED");
      break;
  }
  // 0 WIFI_EVENT_STAMODE_CONNECTED    подключение к роутеру получение ip
  // 1 WIFI_EVENT_STAMODE_DISCONNECTED попытка переподключения к роутеру
  // 2 WIFI_EVENT_STAMODE_AUTHMODE_CHANGE
  // 3 WIFI_EVENT_STAMODE_GOT_IP подключен к роутеру
  // 4 WIFI_EVENT_STAMODE_DHCP_TIMEOUT Не получен адрес DHCP
  // 5 WIFI_EVENT_SOFTAPMODE_STACONNECTED подключен клент
  // 6 WIFI_EVENT_SOFTAPMODE_STADISCONNECTED отключен клент
  // 7 WIFI_EVENT_SOFTAPMODE_PROBEREQRECVED Режим точки доступа
  // 8 WIFI_EVENT_MAX,
  // 9 WIFI_EVENT_ANY = WIFI_EVENT_MAX,
  // 10 WIFI_EVENT_MODE_CHANGE

}
