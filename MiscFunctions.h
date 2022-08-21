/* very cool macro-replacement
 *  https://gcc.gnu.org/onlinedocs/cpp/Stringizing.html#Stringizing
 *  example https://stackoverflow.com/questions/34024437/arduino-how-to-print-out-value-of-defined-constants
    ....
    #define THEVAL 12345
    ....
    Serial.println("The value of " STR(THEVAL) " is " XSTR(THEVAL));

    This would output:
    The value of THEVAL is 12345

 * more to read https://en.cppreference.com/w/cpp/preprocessor/replace
 *  * 
 */
#define STR(s) #s             
#define XSTR(s) STR(s)        

/********************************************************
* Format runtime info
* max: 49d17h2m47s 
********************************************************/
String formatTimeElapsed(unsigned long MillisVal) 
{
    unsigned long rt_sec = MillisVal/1000;
    unsigned long rt_min = rt_sec/60;
    unsigned int rt_h = rt_min/60;
    unsigned int rt_d = rt_h/24;
    
    String ret = String(rt_d) + "d" + String(rt_h - rt_d*24) + "h" + String(rt_min - rt_h*60) + "m" + String(rt_sec - rt_min*60) + "s";
    return ret;
}


float roundFloat(float x, int number_of_digits)
{
  long factor = pow(10,number_of_digits);
  return ((long) (x * ((long)pow(10,number_of_digits)))) / 10.0;
}


void printBinaryByte(byte inByte)
{
  for (int b = 7; b >= 0; b--)
  {
    Serial.print(bitRead(inByte, b));
  }
}

void printBinaryWord(int inByte)
{
  for (int b = 15; b >= 0; b--)
  {
    Serial.print(bitRead(inByte, b));
  }
}
