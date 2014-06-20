//Include statements
#include <Adafruit_BMP085_U.h>
#include <Adafruit_MCP23017.h>
#include <Adafruit_RGBLCDShield.h>
#include <Adafruit_Sensor.h>
#include <SoftwareSerial.h>
#include <String.h>
#include <Wire.h>

//Global Variables
SoftwareSerial mySerial(7, 8);          //software serial connection used to communicate to and from the gprs shield
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();           //initiation of  the rgdlcd shield
Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);  //initiation of the BMP 805 sensor

const int light = 12;            // This is the pin that the leds are connected to in series
int PressNum = 0;                // The number of times the button has been pressed
float temperature;               // Variable for BMP
float seaLevelPressure;          // Variable for BMP
float Altitude;                  // Variable for BMP
float pres;                      // Variable for BMP
boolean Timer=false;             // true means start Timer/ false means do not start Timer
int xTimer=0;                    // This is the counter for the timer to indicate how many 500ms delays has occurred
const String PhoneBook[] = {("\"YourNumberHere\"") , ("\"YourNumberHere\"")};
void setup(void) 
{
  pinMode(light, OUTPUT);
  
  mySerial.begin(19200);               // the GPRS baud rate   
  Serial.begin(9600);                  // The Serial Port baud rate
  lcd.begin(16, 2);
  lcd.setBacklight(0x0);               // Turns off the backlight of the lcd screen
 
  // Initialise the sensor 
  if(!bmp.begin())
  {
    // There was a problem detecting the BMP085 ... check your connections    
    lcd.setBacklight(0x7);             // Turns on the backlight of the lcd screen
    lcd.setCursor(0,0);                // The place where you want the first word to begin setCursor(PLACE,ROW)
    lcd.print("Sensors were not");
    lcd.setCursor(0,1);
    lcd.print("Found");
    while(1);
  }
  FFoff();
  delay(500);
}

void loop(void) 
{ 
  sensors_event_t event;
  bmp.getEvent(&event);
  uint8_t buttons = lcd.readButtons();
  
  if (event.pressure)
  {        
    bmp.getTemperature(&temperature);                   //getTemperature
    seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;    //getPressure
    pres = event.pressure;
    Altitude = bmp.pressureToAltitude(seaLevelPressure, event.pressure, temperature);  //getAltitude

    int num_of_sms = sizeof(PhoneBook)/sizeof(String); // Gets the size of the PhoneBook array
    
    for(int i=0; i<num_of_sms; i++) { SendTextMessage(PhoneBook[i]); }  // For loop to send email to everyone in the PhoneBook array
  }
  else { lcd.clear(); lcd.setCursor(0,0); lcd.print("Sensor error");} //Serial.println("Sensor error"); }

  if(buttons & BUTTON_SELECT)
  {
    if(PressNum%2==0)
    { 
      digitalWrite(light, HIGH);   // turn the LED on (HIGH is the voltage level)
      FFon();
      delay(3500);
      PressNum=PressNum++;  // adds one to PressNum
    }
    else
    {
      digitalWrite(light, LOW);   // turn the LED off (LOW is the voltage level)
      FFoff();
      PressNum=PressNum++;       
    }  

    // Send sensor data to the Lcd
    display_in_lcd("Pressure:",event.pressure,"hPa");
    display_in_lcd("Temperature:",((temperature*9)/5)+32,"F");
    display_in_lcd("Altitude:",(-1*(bmp.pressureToAltitude(seaLevelPressure, event.pressure, temperature))),"m");
    display_in_lcd("Voltage Level:",23,"volts");
        
    ffReport();    
    
    // Send data to the serial monitor on the computer
    /*
    display_in_serial("Temperature",temperature,"C");
    display_in_serial("Pressure",event.pressure,"hPa");
    display_in_serial("Altitude",bmp.pressureToAltitude(seaLevelPressure, event.pressure, temperature),"m");
    */
  }
  turn_off();
}

void turn_off()
{  
  if(Timer)
  {
      delay(500);
      if(xTimer==180)
      { 
         digitalWrite(light, LOW); 
         xTimer=0;
         PressNum=PressNum++;
         Timer=false;
      }
      else 
          xTimer++;       
  }
  else 
    delay(500);
}

void display_in_lcd(String title, float value, String unit)
{
  lcd.setBacklight(0x7);
  lcd.clear();
  lcd.setCursor(0,0);               
  lcd.print(title);
  lcd.setCursor(0,1);
  lcd.print(value);
  lcd.setCursor(8,1);
  lcd.print(unit);
  lcd.setCursor(11,1);
  delay(3500);
}

void display_in_serial(String title, float value, String unit)
{
  Serial.print(title);
  Serial.print(":   ") ;
  Serial.print(value);
  Serial.print(" ");
  Serial.println(unit);  
}
void progressBar()
{
  lcd.clear();
  byte bar1[8] = { 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10};
  lcd.createChar(1, bar1);
  lcd.setCursor (0, 1);
  lcd.print("0%");
  lcd.setCursor (4, 1);
  lcd.print("sending");  
  lcd.setCursor (12, 1);
  lcd.print("100%");
  for (int x = 0; x < 16; x++)                        // Print all the filled blocks
  {
    lcd.setCursor (x, 0);
    lcd.write (1023);
    delay(800/(x+1));
  } 
 delay(500);
}

void FFoff()
{ 
  lcd.clear();
  lcd.setBacklight(0x7);
  lcd.setCursor(1,0);
  lcd.print("Firefly is off");
  lcd.setCursor(4,1);
  lcd.print("at DtM ");  
  delay(3500);
  lcd.setBacklight(0x0);  
  Timer=false;  
}
    
void FFon()
{
  lcd.clear();
  lcd.setBacklight(0x7);
  lcd.setCursor(1,0);
  lcd.print("Firefly is on");
  lcd.setCursor(4,1);
  lcd.print(" at DtM");
  Timer=true;
}
void ffReport()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Sending Report");
  lcd.setCursor(0,1);
  lcd.print("About Firefly! ");
  delay(3500);
  progressBar();
  lcd.clear();
  lcd.setCursor(0,0);

  lcd.print("Report About");
  lcd.setCursor(0,1);
  lcd.print("Firefly was sent!");  
  delay(3500);
  lcd.clear();
  lcd.setBacklight(0x0);
}

///SendTextMessage(String num)
///this function is to send a sms message
void SendTextMessage(String num)
{
   String recipient = "AT + CMGS = ";
   recipient+=num;
   mySerial.print("AT+CMGF=1\r");    //Because we want to send the SMS in text mode
   delay(100); 
   //mySerial.println("AT + CMGS = \"AddNumberHere\"");//send sms message, be careful need to add a country code before the cellphone number
   mySerial.println(recipient);//send sms message, be careful need to add a country code before the cellphone number
   delay(100);
   mySerial.println("A test message!");//the content of the message
   delay(100);
   mySerial.println((char)26);//the ASCII code of the ctrl+z is 26
   delay(100);
   mySerial.println();
}
