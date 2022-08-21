POWER BOX
(c) 2022 by Boris Emchenko

Необходимые библиотеки:


// Стандартные из ядра:
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPClient.h>

//Для работы с OTA
#include <ArduinoOTA.h>
#include <ESP8266HTTPUpdateServer.h>

//Внешния для I2C датчиков
#include <Adafruit_SHT31.h>
#include <MLX90614.h>

#include <Ticker.h>

Wire.h - стандартая версия из пакета
    version=1.0
    author=Arduino
    maintainer=Ivan Grokhotkov <ivan@esp8266.com>
    sentence=Allows the communication between devices or sensors connected via Two Wire Interface Bus. For esp8266 boards.  
    AppData\\Local\\Arduino15\\packages\\esp8266\\hardware\\esp8266\\2.7.4\\libraries\\Wire
	repository=https://github.com/bblanchon/ArduinoJson.git

MCP23017.h -не модифицированная библиотека
	name=MCP23017
	version=2.0.0
	author=Bertrand Lemasle
	maintainer=Bertrand Lemasle
	sentence=MCP23017 I2C Port expander library.

GyverINA.h - МОДИФИЦИРОВАННАЯ библиотека
	v1.2b (based on 1.2) - "test connection" made public

IotWebConf.h - не модифицированная библиотека
	version=3.2.0
	author=Balazs Kelemen <prampec+arduino@gmail.com>
	maintainer=Balazs Kelemen <prampec+arduino@gmail.com>
	sentence=ESP8266/ESP32 non-blocking WiFi/AP web configuration.
	paragraph=IotWebConf will start up in AP (access point) mode, and provide a config portal for entering WiFi connection and other user-settings. The configuration is persisted in EEPROM. The config portal will stay available after WiFi connection was made. A WiFiManager alternative.
	url=https://github.com/prampec/IotWebConf

 MLX90614.h - МОДИФИЦИРОВАННАЯ библиотека
	 1.01   [18.07.2020]    added SDA and SCK pins configuration to begin method


****************************************************************************************************************************************

Hardware:
    Wemos D1 mini
    Настройки компиляции: 4MB (FS 2MB, OTA 1 MB)
    