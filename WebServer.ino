/*
 * TEMPLATE HEADER
 */
const char HTTP_HTML_HEADER[] PROGMEM = "<!DOCTYPE html>\
<html>\
<head>\
<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\
<meta charset=\"utf-8\">\
<title>Power Bro Lite</title>\n\
<style>\
table {  width: 100%;  max-width: 400px;   min-width: 200px; border: 1px solid #1C6EA4;  text-align: center;  border-collapse: collapse;  margin-left:auto;   margin-right:auto; }\n\
table td, table th {  border: 1px solid #AAAAAA;  padding: 3px 2px; min-width: 30px }\n\
table thead {  background: #2672B5;  background: -moz-linear-gradient(top, #5c95c7 0%, #3b80bc 66%, #2672B5 100%);   background: -webkit-linear-gradient(top, #5c95c7 0%, #3b80bc 66%, #2672B5 100%);   background: linear-gradient(to bottom, #5c95c7 0%, #3b80bc 66%, #2672B5 100%);  border-bottom: 1px solid #000000;  font-size: 15px;  font-weight: bold;  color: #FFFFFF; }\n\
table tbody td {  font-size: 13px; }\n\
.footer { font-size:10px }\n\
button { min-width: 50px }\n\
input  { width: 50px }\n\
</style>\n\
<script></script>\n\
</head>\n\
<body style=\"text-align:center\" onload='load()'>\n\
<h1>Power Bro Lite</h1>\
<p id='connectionstat'></p>\
\n";

/*
 * TEMPLATE FOOTER
 */
const char HTTP_HTML_FOOTER[] PROGMEM = "<p class='footer'>PowerBro Lite v{Ver} [{VDate}]. Passed since start: <span class='footer'><span id='RTM'>{RTM}</span><br>Free mem: <span id='MEM'>{MEM}</span> | SSID: <span id='SSID'>{SSID}</span> | RSSI: <span id='NTR'>{NTR}</span> | WiFi channel: <span id='NTC'>{NTC}</span> | IP: <span id='IP'>{IP}</span> | MAC: <span id='MAC'>{MAC}</span></p><p class='footer'><a href=/config>CONFIG</a> | <a href='javascript:;' onclick=\"commandSend('"CMD_ESP_REBOOT"');\">REBOOT</a> | <a href=/update>UPDATE</a></p><div id='jdebug'></div><div id='debug'></div><p id='debug2'></p>";
const char HTTP_HTML_END[] PROGMEM = "</body></html>";

/*
 RELAY TABLE TEMPLATE
 */
const char HTTP_HTML_RELAYTABLE1[] PROGMEM = "<h3>Power</h3><table><thead>\
        <tr>\
          <th>Out 1 [5A]</th><th>Out 2 [5A]</th><th>Out 3 [3A]</th><th>Out 4 [3A]</th><th>Out 5 [NOL]</th><th>PWMs</th>\
        </tr>\
    </thead><tbody>\n\
      <tr>\
        <td id='RL11'>{RL11}</td>\
        <td id='RL12'>{RL12}</td>\
        <td id='RL13'>{RL13}</td>\
        <td id='RL14'>{RL14}</td>\
        <td id='RL15'>{RL15}</td>\
        <td id='RL16'>{RL16}</td>\
      </tr>\n\
      <tr>\
        <td><button onclick=\"relaysend(11,1);\">ON</button><button onclick=\"relaysend(11,0);\">OFF</button></td>\
        <td><button onclick=\"relaysend(12,1);\">ON</button><button onclick=\"relaysend(12,0);\">OFF</button></td>\
        <td><button onclick=\"relaysend(13,1);\">ON</button><button onclick=\"relaysend(13,0);\">OFF</button></td>\
        <td><button onclick=\"relaysend(14,1);\">ON</button><button onclick=\"relaysend(14,0);\">OFF</button></td>\
        <td><button onclick=\"relaysend(15,1);\">ON</button><button onclick=\"relaysend(15,0);\">OFF</button></td>\
        <td><button onclick=\"relaysend(16,1);\">ON</button><button onclick=\"relaysend(16,0);\">OFF</button></td>\
      </tr>\n\
    </tbody></table>\n";

/*
 INA SENSORS TABLE TEMPLATE
 */
const char HTTP_HTML_SENSTABLE[] PROGMEM = "<h3>Voltage/Current</h3><table><thead>\
      <tr>\
        <th></th><th>Mains</th><th>Out1</th><th>Out2</th><th>Out3</th><th>Out4</th><th>PWM</th><th>USB</th>\
      </tr>\
    </thead><tbody>\n\
      <tr>\
        <td>Voltage (V)</td>\
        <td id='VL0'>{VL0}</td>\
      </tr>\n\
      <tr>\
        <td>Current (A)</td>\
        <td id='CR0'>{CR0}</td>\
      </tr>\n\
      <tr>\
        <td>Power (W)</td>\
        <td id='PW0'>{PW0}</td>\
      </tr>\n\
    </tbody></table>\n";

const char HTTP_HTML_PWM[] PROGMEM = "<h3>PWM</h3><table><tbody>\n\
      <tr>\
        <td>PWM1</td>\
        <td id='PWM1'>{PWM1}</td>\
        <td><input id=\"PWM1set\" onfocus=\"pwm1edit = true\" onblur=\"pwm1edit = false\"> [0..1023]</td>\
        <td><button onclick=\"setPWM(1, document.getElementById('PWM1set').value);\">SET</button></td>\
      </tr>\n\
      <tr>\
        <td>PWM2</td>\
        <td id='PWM2'>{PWM2}</td>\
        <td><input id=\"PWM2set\"  onfocus=\"pwm2edit = true\" onblur=\"pwm2edit = false\"> [0..1023]</td>\
        <td><button onclick=\"setPWM(2, document.getElementById('PWM2set').value);\">SET</button></td>\
      </tr>\n\
      <tr>\
        <td>PWM3</td>\
        <td id='PWM3'>{PWM3}</td>\
        <td><input id=\"PWM3set\"  onfocus=\"pwm3edit = true\" onblur=\"pwm3edit = false\"> [0..1023]</td>\
        <td><button onclick=\"setPWM(3, document.getElementById('PWM3set').value);\">SET</button></td>\
      </tr>\n\
    </tbody></table>\n";


/*
 SENSORS
 */
const char HTTP_HTML_SENSORS_HEAD[] PROGMEM = "<h3>Sensors</h3><table><tbody>\n";
const char HTTP_HTML_SENSORS_FOOTER[] PROGMEM = "</tbody></table>\n";

#ifdef ENABLE_SHT31
const char HTTP_HTML_SENSORS_SHT[] PROGMEM = "\
      <tr>\
        <td>SHT31 temperature</td>\
        <td><span id='"SENSOR_SHT_TEMPERATURE"'>{"SENSOR_SHT_TEMPERATURE"}</span>° (heater: <span id='"SENSOR_SHT_HEATER_STATUS"'>{"SENSOR_SHT_HEATER_STATUS"}</span>) [<a href='javascript:;' onclick=\"commandSend('"CMD_SHT_HEATER_ON"');\">ON</a>][<a href='javascript:;' onclick=\"commandSend('"CMD_SHT_HEATER_OFF"');\">OFF</a>]</td>\
      </tr>\n\
      <tr>\
        <td>SHT31 humidity</td>\
        <td><span id='"SENSOR_SHT_HUMIDITY"'>{"SENSOR_SHT_HUMIDITY"}</span>%</td>\
      </tr>\n";
#endif

#ifdef ENABLE_MLX90614
const char HTTP_HTML_SENSORS_MLX[] PROGMEM = "\
      <tr>\
        <td>MLX90614 obj temp</td>\
        <td><span id='"SENSOR_MLX_OBJECT"'>{"SENSOR_MLX_OBJECT"}</span>°</td>\
      </tr>\n\
      <tr>\
        <td>MLX90614 amb temp</td>\
        <td><span id='"SENSOR_MLX_AMBIENT"'>{"SENSOR_MLX_AMBIENT"}</span>°</td>\
      </tr>\n\
      <tr>\
        <td>MLX90614 obj temp</td>\
        <td><span id='"SENSOR_MLX_CLOUD"'>{"SENSOR_MLX_CLOUD"}</span></td>\
      </tr>\n";
#endif

/*
 ERROR DISPLAYING
 */
const char HTTP_HTML_ERROR[] PROGMEM = "<h3>Self diagnostics</h3><table>\n\
      <tr>\
        <td colspan=12>Error code: <span id='ERR'>{ERR}</span></td>\
      </tr>\n\
      <thead>\
        <tr>\
          <th>MCP1</th>\
          <th>INA</th>\
          <th>MEM</th>\
          <th>MLX</th>\
          <th>SHT</th>\
          <th>-</th>\
          <th>INA0</th>\
        </tr>\
    </thead><tbody>\n\
      <tr>\
        <td id='ERR0'>0</td>\
        <td id='ERR1'>0</td>\
        <td id='ERR2'>0</td>\
        <td id='ERR3'>0</td>\
        <td id='ERR4'>0</td>\
        <td>-</td>\
        <td id='ERRINA0'>0</td>\
      </tr>\n\
    </tbody></table>\n";
   
/*
 * AUTOUPDATE DATA
 */
const char HTTP_HTML_UPDATE_MAIN1[] PROGMEM = "<script>\
    window.setInterval(\"update()\", {update});\
    var pwm1edit = false, pwm2edit = false, pwm3edit = false;\
    \
    function update(){\
      var xhr=new XMLHttpRequest();\
      xhr.open(\"GET\", \"/json\", true);\
      xhr.onreadystatechange = function () {\
        if (xhr.readyState != XMLHttpRequest.DONE || xhr.status != 200) { ConnectionError(xhr.readyState, xhr.status); return; }\
        document.getElementById('connectionstat').innerHTML='';\
        var getData = JSON.parse(xhr.responseText);\n\
        for( i=1; i<= 6; i++){\
          document.getElementById('RL1'+i).innerHTML=(getData['"SENSOR_RELAY_STATUS(1)"'+i] == 1 ? 'ON' : 'OFF');\
          document.getElementById('RL1'+i).style.backgroundColor = (getData['"SENSOR_RELAY_STATUS(1)"'+i] == 1? '#26b569' :'#b5262b');\
        }\n\
        document.getElementById('PWM1').innerHTML=getData['"SENSOR_PWM(1)"'];\
        if (!pwm1edit) document.getElementById('PWM1set').value=getData['"SENSOR_PWM(1)"'];\
        document.getElementById('PWM2').innerHTML=getData['"SENSOR_PWM(2)"'];\
        if (!pwm2edit) document.getElementById('PWM2set').value=getData['"SENSOR_PWM(2)"'];\
        document.getElementById('PWM3').innerHTML=getData['"SENSOR_PWM(3)"'];\
        if (!pwm3edit) document.getElementById('PWM3set').value=getData['"SENSOR_PWM(3)"'];\n\
        for( i=0; i< " XSTR(INA_COUNT) "; i++){\
          document.getElementById('VL'+i).innerHTML=getData['"SENSOR_INA_VOLTAGE_PREFIX"'+i];\
          document.getElementById('CR'+i).innerHTML=getData['"SENSOR_INA_CURRENT_PREFIX"'+i];\
          document.getElementById('PW'+i).innerHTML=getData['"SENSOR_INA_POWER_PREFIX"'+i];\
        }\n\
        document.getElementById('ERR').innerHTML=getData['"SENSOR_ERROR_STATUS"'] + ' [' + getData['"SENSOR_ERROR_DETAILS_STRING"'] + ']';\
        for (i=0; i<=4; i++){\
          document.getElementById('ERR'+i).innerHTML=getData['"SENSOR_ERROR_DETAILS_STRING"'].charAt(i);\
          document.getElementById('ERR'+i).style.backgroundColor = ((getData['"SENSOR_ERROR_DETAILS_STRING"'].charAt(i)=='1') ? '#b5262b' : '#FFFFFF');\
        }\n\
        for (i=0; i<"XSTR(INA_COUNT)"; i++){\
          document.getElementById('ERRINA'+i).innerHTML=getData['"SENSOR_ERROR_DETAILS_STRING"'].charAt(i+6);\
        }\n\
        document.getElementById('RTM').innerHTML=getData['" SENSOR_ESP_RUNTIME "'];\
        document.getElementById('MEM').innerHTML=getData['" SENSOR_ESP_FREE_MEMORY "'];\
        document.getElementById('NTR').innerHTML=getData['" SENSOR_NETWORK_WIFI_RSSI "'];\
        document.getElementById('NTC').innerHTML=getData['" SENSOR_NETWORK_WIFI_CHANNEL "'];\
        document.getElementById('debug').innerHTML=document.getElementById('debug').innerHTML + (getData.debug !='' ? '<br>' + getData.debug : '');\n";

const char HTTP_HTML_UPDATE_MAIN2[] PROGMEM = "\
      };\
      xhr.send();\
    }\n\
    \
    function relaysend(rnum, rstat) {\
      commandSend('"CMD_SWITCH_RELAY"'+rnum, rstat);\
    }\n\
    \
    function relaytoggle(rnum) {\
      relaysend(rnum, 0);\
      setTimeout(relaysend,4000,rnum,1);\
    }\n\
    \
    function setcolor(rnum) {\
      commandSend('"CMD_LED_COLOR"',rnum);\
    }\n\
    \
    function setPWM(outnum, outlevel){\
      commandSend('"CMD_PWM_LEVEL"'+outnum, outlevel);\
    }\n\
    \
    function commandSend(cmd_name, cmd_arg='') {\
      var xhr=new XMLHttpRequest();\
      xhr.open(\"GET\", '/cmd?' + cmd_name + '=' + cmd_arg, true);\
      xhr.onreadystatechange = function () {\
        if (xhr.readyState != XMLHttpRequest.DONE || xhr.status != 200) return;\
        var getData = xhr.responseText;\
        document.getElementById('jdebug').innerHTML=getData;\
        update();\
      };\
      xhr.send();\
    }\n\
    \
    function ConnectionError(xhrstate, xhrstatus) {\
        document.getElementById('connectionstat').innerHTML='CONNECTION ERROR (state:' + xhrstate + ', status:' +  xhrstatus + ')';\
    }\n\
    \
    function load(){\
      update();\
    }\n\
    </script>\n";

#ifdef ENABLE_SHT31
const char HTTP_HTML_UPDATE_SHT[] PROGMEM = "\
        document.getElementById('"SENSOR_SHT_TEMPERATURE"').innerHTML=getData['"SENSOR_SHT_TEMPERATURE"'];\
        document.getElementById('"SENSOR_SHT_HUMIDITY"').innerHTML=getData['"SENSOR_SHT_HUMIDITY"'];\
        document.getElementById('"SENSOR_SHT_HEATER_STATUS"').innerHTML=(getData['"SENSOR_SHT_HEATER_STATUS"'] == '"XSTR(SHT_HEATER_ON)"' ? 'on': (getData['"SENSOR_SHT_HEATER_STATUS"'] == '"XSTR(SHT_HEATER_OFF)"' ? 'off': 'cooldown'));\
        document.getElementById('"SENSOR_SHT_TEMPERATURE"').style.color = ((getData['"SENSOR_ERROR_DETAILS_STRING"'].charAt(5)=='1') ? '#D3D3D3' : '#000000');\
        document.getElementById('"SENSOR_SHT_HUMIDITY"').style.color = ((getData['"SENSOR_ERROR_DETAILS_STRING"'].charAt(5)=='1') ? '#D3D3D3' : '#000000');\
        document.getElementById('"SENSOR_SHT_HEATER_STATUS"').style.color = ((getData['"SENSOR_ERROR_DETAILS_STRING"'].charAt(5)=='1') ? '#D3D3D3' : '#000000');\n";
#endif
#ifdef ENABLE_MLX90614
const char HTTP_HTML_UPDATE_MLX[] PROGMEM = "\
        document.getElementById('"SENSOR_MLX_OBJECT"').innerHTML=getData['"SENSOR_MLX_OBJECT"'];\
        document.getElementById('"SENSOR_MLX_AMBIENT"').innerHTML=getData['"SENSOR_MLX_AMBIENT"'];\
        document.getElementById('"SENSOR_MLX_CLOUD"').innerHTML=getData['"SENSOR_MLX_CLOUD"'];\        
        document.getElementById('"SENSOR_MLX_OBJECT"').style.color = ((getData['"SENSOR_ERROR_DETAILS_STRING"'].charAt(4)=='1') ? '#D3D3D3' : '#000000');\
        document.getElementById('"SENSOR_MLX_AMBIENT"').style.color = ((getData['"SENSOR_ERROR_DETAILS_STRING"'].charAt(4)=='1') ? '#D3D3D3' : '#000000');\
        document.getElementById('"SENSOR_MLX_CLOUD"').style.color = ((getData['"SENSOR_MLX_CLOUD"']=='"XSTR(TEMPERATURE_UNKNOWN_VALUE)"') ? '#D3D3D3' : '#000000');\n";
#endif

const char HTTP_HTML_REDIRECT[] PROGMEM = "<script>\
    window.setTimeout(\"update()\", {update});\
    function update(){\
        window.location.href = 'http://{self_url}/';\
    }\
  </script>";


/*
 * ROOT PAGE
 */
void handleRoot() {
  digitalWrite(STATUS_LED, HIGH);

  // Let IotWebConf test and handle captive portal requests.
  if (iotWebConf.handleCaptivePortal()) {
    return; // Captive portal request were already served.
  }

  String page, page1, pagescr;

  IPAddress ServerIP;
  String s = "192.168.4.1";
  ServerIP.fromString(s);

  if ( ServerIP == WiFi.softAPIP())
  {
      page = "<!DOCTYPE html><html lang=\"en\"><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1, user-scalable=no\"/>";
      page += "<title>PowerBro Config Portal</title></head><body>";
      page += "Go to <a href='config'>configure page</a> to change settings.";
      page += "</body></html>\n";
  } else {
    page = FPSTR(HTTP_HTML_HEADER);
    page1 = FPSTR(HTTP_HTML_RELAYTABLE1);
    page +=page1;
    page1 = FPSTR(HTTP_HTML_SENSTABLE);
    page +=page1;
    page1 = FPSTR(HTTP_HTML_PWM);
    page +=page1;

    page1 = FPSTR(HTTP_HTML_SENSORS_HEAD);
    page +=page1;
    #ifdef ENABLE_SHT31
      page1 = FPSTR(HTTP_HTML_SENSORS_SHT);
      page +=page1;
    #endif
    #ifdef ENABLE_MLX90614
      page1 = FPSTR(HTTP_HTML_SENSORS_MLX);
      page +=page1;
    #endif
    page1 = FPSTR(HTTP_HTML_SENSORS_FOOTER);
    page +=page1;
    
    page1 = FPSTR(HTTP_HTML_ERROR);
    page +=page1;
  
    page1 = FPSTR(HTTP_HTML_FOOTER);
    page +=page1;
    page1 = FPSTR(HTTP_HTML_END);
    page +=page1;
  
    page.replace("{Ver}", String(VERSION));
    page.replace("{VDate}", String(VERSION_DATE));
    page.replace("{SSID}", String(WiFi.SSID()));
    page.replace("{IP}", WiFi.localIP().toString());
    page.replace("{MAC}", WiFi.macAddress());

    pagescr = FPSTR(HTTP_HTML_UPDATE_MAIN1);
    #ifdef ENABLE_SHT31
      page1 = FPSTR(HTTP_HTML_UPDATE_SHT);
      pagescr +=page1;
    #endif
    #ifdef ENABLE_MLX90614
      page1 = FPSTR(HTTP_HTML_UPDATE_MLX);
      pagescr +=page1;
    #endif
    page1 = FPSTR(HTTP_HTML_UPDATE_MAIN2);
    pagescr +=page1;
  
    page.replace("<script></script>", pagescr);
    page.replace("{update}", String(JS_UPDATEDATA_INTERVAL));
  }
  
  server.send(200, "text/html", page);

  printRequestData();
  digitalWrite(STATUS_LED, LOW);
}

 /*
 * JSON Sensor Status Page
 */
void handleJSON(){
  digitalWrite(STATUS_LED, HIGH);

  // -- Let IotWebConf test and handle captive portal requests.
  if (iotWebConf.handleCaptivePortal())
  {
    // -- Captive portal request were already served.
    return;
  }

  String page = SensorsJSON();
  
  server.send(200, "application/json", page);

  printRequestData();
  digitalWrite(STATUS_LED, LOW);
}


void handlePingRequest(){
  digitalWrite(STATUS_LED, HIGH);
  String page = "OK"; //returned data
  server.send(200, "text/plain", page);
  printRequestData();
  digitalWrite(STATUS_LED, LOW);
}

/*
 *  Print HTTP request data
 */
void printRequestData()
{
  Serial.print(F("[HTTP REQUEST] client: "));
  Serial.print(server.client().remoteIP().toString());
  //Serial.print("[HTTP REQUEST] method: ");
  //Serial.println(server.method());
  Serial.println(" URI: " + server.uri());
}

void handleCommands() 
{
  digitalWrite(STATUS_LED, HIGH);

  // Serial debug output
  // sever string format: relay1=1
  Serial.print(F("Command: "));
  Serial.println(server.argName(0));
  Serial.print(F("Argument: "));
  Serial.println(server.arg(0));
  
  parseCommand(server.argName(0), server.arg(0));

  //web output message
  String message = "Command [" + server.argName(0) + "=" + server.arg(0) + "] exectuted";
  server.send(200, "text/plain", message);
  
  printRequestData();
  digitalWrite(STATUS_LED, LOW);
}

/*
 * GET part of REST API
 * 
 * returns current relay state in format, compatible with Astrohostel API
 * Example of return: {"success":true,"data":{"107":{"name":"\u0420\u0435\u043b\u0435 1","state":0},"201":{"name":"\u0420\u0435\u043b\u0435 2","state":0},"198":{"name":"\u0411\u043e\u0440\u0438\u0441-\u041a\u043e\u043c\u043f","state":1},"199":{"name":"\u0411\u043e\u0440\u0438\u0441-\u0410\u0441\u0442\u0440\u043e\u0433\u0440\u0430\u0444","state":0}}}
 * 
 */
void handleREST_GET() 
{
  digitalWrite(STATUS_LED, HIGH);
  // -- Let IotWebConf test and handle captive portal requests.
  if (iotWebConf.handleCaptivePortal())
  {
    return;     // -- Captive portal request were already served.
  }

  String page = "{";
  page += "\"success\": " + String(hardwareERRstatus == 0? "true" : "false") + ", \"data\": { ";
  for(int i=1; i<=6; i++) {
    page += "\"" SENSOR_RELAY_STATUS(1) + String(i) + "\": {\"name\": \"" SENSOR_RELAY_STATUS(1) + String(i) + "\", \"state\": " + String(getRelayStatus(1,i)) + "}" + (i<6?String(", "):"");
  }
  page +=" }";  //data block
  page +="}";  

  server.send(200, "application/json", page);
  Serial.println(F("[REST API] GET query"));
  
  printRequestData();
  digitalWrite(STATUS_LED, LOW);
}
/*
 * POST part of REST API
 * 
 * Example from Astrohostel API (Set):
 * sUrl = "http://192.168.2.99/REST/relay/";
 * sRequest = "gpio=199&state=1";
 * Method POST
 * Response: {"success":true,"data":{"gpio":199,"state":1}}
 * 
 * Our example:
 * gpio=RL11&state=1
 * Response: {"success":true,"data":{"gpio":"RL11","state":1}}
 *           {"success":true,"data":{"RL15": {"name": "RL15", "state": 1} }}
 */
void handleREST_POST() 
{
  digitalWrite(STATUS_LED, HIGH);
  // -- Let IotWebConf test and handle captive portal requests.
  if (iotWebConf.handleCaptivePortal())
  {
    return;     // -- Captive portal request were already served.
  }

  // scan all parameters
  for (int i = 0; i < server.args(); i++) 
  {
    Serial.print("Arg nº" + (String)i + " –>");
    Serial.print(server.argName(i) + ": ");
    Serial.println(server.arg(i));
  } 
  bool QuerySuccess = true;
  String relayName = server.arg("gpio");
  String relayState = server.arg("state");
  int relayN = relayName.substring(3,4).toInt();

  Serial.print("GPIO Num: ");
  Serial.print(relayName);
  Serial.print(" -> ");
  Serial.println(relayName.substring(3,4));
  if (relayN <= 0 || relayN > 6) {
    QuerySuccess = false;
  } else {
    if (relayState == "1") {
      RelayOn(1,relayN);
    } else if (relayState == "0") {
      RelayOff(1,relayN);
    } else {
      QuerySuccess = false;
    }
  } 
  String page = "{";
  page += "\"success\": " + String(QuerySuccess? "true" : "false") + ", \"data\": { ";
  page += "\"gpio\": \"" SENSOR_RELAY_STATUS(1) + String(relayN) + "\", \"state\": " + String(getRelayStatus(1,relayN));
  page +=" }";  //data block
  page +="}";  

  server.send(200, "application/json", page);
  Serial.println(F("[REST API] POST query"));
  
  printRequestData();
  digitalWrite(STATUS_LED, LOW);
}
