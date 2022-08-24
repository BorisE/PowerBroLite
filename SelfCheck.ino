/* 
 * Return status (also hardwareERRstatus)
 * 0 - ok         SELFCHECK_OK
 * 1 - minor      SELFCHECK_MINOR
 * 2 - warning    SELFCHECK_WARNING_ERROR
 * 10 - critical  SELFCHECK_CRITICAL_ERROR
 */


/* 
 *  Self check for hardware
 *  
 *  return max error type, stored at global var [hardwareERRstatus]
 *  also print it out as [ERR] sensor value
 *  
 *  stores flags of every checked device at global var [hardwareERRstring]
 *  also print it out [ERS] sensor value
 *  format: [000022-0000000]
 *      [0] checkStatusMCP1
 *      [1] checkStatusMCP2
 *      [2] checkStatusINA
 *      [3] checkStatusMEM
 *      [4] checkStatusMLX - 2 if absent in configuration
 *      [5] checkStatusSHT - 2 if absent in configuration
 *      - 
 *      INA0...INA6
 */
/* new format [!ERS:00022-0]
 *    [0] checkStatusMCP1
 *    [0] checkStatusINA
 *    [0] checkStatusMEM
 *    [0] checkStatusMEM
 *    [2] checkStatusMLX
 *    [2] checkStatusSHT
 *    -
 *    INA0
 */

int selfCheckHardware()
{
    int retStatus =                 SELFCHECK_OK;
    byte error =0;

    Serial.print(F("Running internal self test... "));
    
    /* 1.Check MCP 1 present */
    checkStatusMCP1= check_MCP_1();
    error = checkStatusMCP1;
    if (error == 1) {
      Serial.println(F("!!! MCP23017_1 not found !!!"));
      retStatus = max(retStatus,    SELFCHECK_CRITICAL_ERROR);
    } else if (error == 2) {
      Serial.println(F("!!! MCP23017_1 malfunction (test pin not set) !!!"));
      retStatus = max(retStatus,    SELFCHECK_CRITICAL_ERROR);
    } else if (error > 0) {
      Serial.println(F("!!! MCP23017_1 unknown error !!!"));
      retStatus = max(retStatus,    SELFCHECK_WARNING_ERROR);
    }

    /* 3.Check INA */
    String INA_string = "";
    checkStatusINA = 0;
    for(int i=0; i<INA_COUNT; i++)
    {
      if(!INA_OBJ[i].testConnection()) {
        checkStatusINA = 1;
        Serial.print(F("!!! INA ["));
        Serial.print(i);
        Serial.println(F("] not found !!!"));
        INA_string +="1";
        retStatus = max(retStatus,    SELFCHECK_WARNING_ERROR);
      }
      INA_string +="0";
    }

    /* 4.Check MLX90614 */
    #ifdef ENABLE_MLX90614
    checkStatusMLX = check_MLX();
    if (checkStatusMLX == 1) {
      Serial.println(F("!!! MLX90614 not found !!!"));
      retStatus = max(retStatus,    SELFCHECK_WARNING_ERROR);
    } else if (checkStatusMLX == 2) {
      Serial.println(F("!!! MLX90614 malfunction (wrong fingeprint) !!!"));
      retStatus = max(retStatus,    SELFCHECK_WARNING_ERROR);
    } else if (checkStatusMLX == 3) {
      Serial.println(F("!!! MLX90614 malfunction (wrong measured values) !!!"));
      retStatus = max(retStatus,    SELFCHECK_WARNING_ERROR);
    } else if (checkStatusMLX > 0) {
      Serial.println(F("!!! MLX90614 unknown error !!!"));
      retStatus = max(retStatus,    SELFCHECK_WARNING_ERROR);
    }
    #endif


    #ifdef ENABLE_SHT31
    checkStatusSHT = check_SHT(); 
    if (checkStatusSHT > 0) {
      Serial.println(F("!!! SHT31 error !!!"));
      retStatus = max(retStatus,    SELFCHECK_WARNING_ERROR);
    } else if (checkStatusSHT == 3) {
      Serial.println(F("!!! SHT31 malfunction (wrong measured values) !!!"));
      retStatus = max(retStatus,    SELFCHECK_WARNING_ERROR);
    }
    #endif

    /* 4.Check free memory */
    if (ESP.getFreeHeap()<2000) {
      checkStatusMEM = 1;
      Serial.println(F("!!! LOW MEMORY !!!"));
      retStatus = max(retStatus,    SELFCHECK_CRITICAL_ERROR);
    }

    /* Final message */
    if (retStatus == SELFCHECK_OK) {
      Serial.println(F("OK"));
    } else {
      Serial.print(F("Errors detected!"));
      Serial.println();
    }
    
    hardwareERRstatus = retStatus;
    hardwareERRstring = String(checkStatusMCP1);
    hardwareERRstring += String(checkStatusINA);
    hardwareERRstring += String(checkStatusMEM);
    hardwareERRstring += String(checkStatusMLX);
    hardwareERRstring += String(checkStatusSHT);
    hardwareERRstring += "-" + INA_string;

    return retStatus;
}

// Output error status and error codes
void Output_Selfcheck_Results()
{
    Serial.print(SENSOR_OUT_START);
    Serial.print(SENSOR_ERROR_STATUS ":");
    Serial.print(hardwareERRstatus);
    Serial.println(SENSOR_OUT_END);

    Serial.print(SENSOR_OUT_START);
    Serial.print(SENSOR_ERROR_DETAILS_STRING ":");
    Serial.print(hardwareERRstring);
    Serial.println(SENSOR_OUT_END);
}

uint8_t check_MCP_1()
{
    // 1.Check MCP 1 present
    Wire.beginTransmission(MCP_1_ADDR);
    byte error = Wire.endTransmission();
    if (error > 0) {
      return 1;
    }

    // check if the test pin is still set
    uint8_t readpin = mcp1.digitalRead(MCP1_TEST_PIN);
    //Serial.print("readpin=");
    //Serial.println(readpin);
    if (readpin==0) {
      return 2;
    }

    return 0;
}

#ifdef ENABLE_MLX90614
uint8_t check_MLX()
{
    // 1.Check MLX present
    Wire.beginTransmission(MLX90614_I2C_ADDR);
    byte error = Wire.endTransmission();
    if (error > 0) {
      return 1;
    }
    
    // 2.Check if this is MLX fingeprint
    if (mlx.testMLX() != 1) {
      return 2;
    }

    // 3. Check output values
    if (MLX_obj < TEMPERATURE_MIN_VALUE || MLX_obj > TEMPERATURE_MAX_VALUE || MLX_amb < TEMPERATURE_MIN_VALUE || MLX_amb > TEMPERATURE_MAX_VALUE) {
      return 3;
    }
    /*
    uint8_t testread2 = mlx.testMLX()
    uint16_t testread3 = (mlx.read16(0x2E) & 0xFF);
    Serial.print("MLX test [");
    Serial.print(testread);
    Serial.print("-");
    Serial.print(testread3, HEX);
    Serial.print("-");
    Serial.print(testread2, HEX);
    Serial.println("]");
    */
    return 0;
}
#endif

#ifdef ENABLE_SHT31
uint8_t check_SHT()
{
    if (SHT_ReadErrorsCount > SHT31_CONTINIOUS_ERRORS_TOLERANCE)
      return 1;
    // 2. Check output values
    if (SHT_T < TEMPERATURE_MIN_VALUE || SHT_T > TEMPERATURE_MAX_VALUE || SHT_H < HUMIDITY_MIN_VALUE || SHT_H > HUMIDITY_MAX_VALUE){
      return 2;
    }
    return 0;
}
#endif

void printBoardInfo() {
/*
  SPIFFS.begin();
  FSInfo fs_info;
  SPIFFS.info(fs_info);

  float fileTotalKB = (float)fs_info.totalBytes / 1024.0; 
  float fileUsedKB = (float)fs_info.usedBytes / 1024.0; 
*/  
  float flashChipSize = (float)ESP.getFlashChipSize() / 1024.0 / 1024.0;
  float realFlashChipSize = (float)ESP.getFlashChipRealSize() / 1024.0 / 1024.0;
  float flashFreq = (float)ESP.getFlashChipSpeed() / 1000.0 / 1000.0;
  FlashMode_t ideMode = ESP.getFlashChipMode();
  
  Serial.printf("\n#####################\n");

  Serial.println("Firmware: ");
  Serial.printf("    Chip Id: %08X\n", ESP.getChipId());
  Serial.print("    Core version: "); Serial.println(ESP.getCoreVersion());
  Serial.print("    SDK version: "); Serial.println(ESP.getSdkVersion());
  Serial.print("    Boot version: "); Serial.println(ESP.getBootVersion());
  Serial.print("    Boot mode: "); Serial.println(ESP.getBootMode());

  
  Serial.printf("__________________________\n\n");
  
  Serial.println("Flash chip information: ");
  Serial.printf("    Flash chip Id: %08X (for example: Id=001640E0  Manuf=E0, Device=4016 (swap bytes))\n", ESP.getFlashChipId());
  Serial.printf("    Sketch thinks Flash RAM is size: ");  Serial.print(flashChipSize); Serial.println(" MB");
  Serial.print("    Actual size based on chip Id: "); Serial.print(realFlashChipSize); Serial.println(" MB ... given by (2^( \"Device\" - 1) / 8 / 1024");
  Serial.print("    Flash frequency: "); Serial.print(flashFreq); Serial.println(" MHz");
  Serial.printf("    Flash write mode: %s\n", (ideMode == FM_QIO ? "QIO" : ideMode == FM_QOUT ? "QOUT" : ideMode == FM_DIO ? "DIO" : ideMode == FM_DOUT ? "DOUT" : "UNKNOWN"));

  Serial.printf("__________________________\n\n");

  Serial.printf("CPU frequency: %u MHz\n\n", ESP.getCpuFreqMHz());

/*
  Serial.printf("__________________________\n\n");

  Serial.println("File system (SPIFFS): ");
  Serial.print("    Total KB: "); Serial.print(fileTotalKB); Serial.println(" KB");
  Serial.print("    Used KB: "); Serial.print(fileUsedKB); Serial.println(" KB");
  Serial.printf("    Block size: %lu\n", fs_info.blockSize);
  Serial.printf("    Page size: %lu\n", fs_info.pageSize);
  Serial.printf("    Maximum open files: %lu\n", fs_info.maxOpenFiles);
  Serial.printf("    Maximum path length: %lu\n\n", fs_info.maxPathLength);

  Dir dir = SPIFFS.openDir("/");
  Serial.println("SPIFFS directory {/} :");
  while (dir.next()) {
    Serial.print("  "); Serial.println(dir.fileName());
  }
*/
  Serial.println("#####################");
}

/*
 * Handle critical error
 * 
 * current algorigthm: 
 * - if not connected to WiFi (i.e. you can use manual rebooting)
 * - if MCP1 is not working (MCP1 switches on/off 12V outputs)
 * - if 100 sec passed since first critical error detecting passed
 * then reboot
  */
void selfCheck_HandleCrtiticalError()
{
    StatusLed_TwoShort_Start();
    Serial.println(F("*** Crtitcal error detected!!! ***"));
    // if not connected to WiFi and wait for 30 sec - reboot and MCP1 with power relays doesn't working
    if (WiFi.status() != WL_CONNECTED && _CriticalError_detected > MAX_CRITICALERRORS_BEFORE_REBOOT && checkStatusMCP1 !=0)
    {
      Serial.println(F("*** Rebooting ***"));
      delay(1000);
      ESP.restart(); // restart if critical error detected in cycle
    }
}
