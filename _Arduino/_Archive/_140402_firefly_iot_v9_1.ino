//Include statements
#include <Adafruit_BMP085_U.h>
#include <Adafruit_MCP23017.h>
#include <Adafruit_RGBLCDShield.h>
#include <Adafruit_Sensor.h>
#include <Firefly.h>
#include <SoftwareSerial.h>
#include <String.h>
#include <Wire.h>
#include <EEPROM.h>

//Global Variables
SoftwareSerial mySerial(7, 8);          //software serial connection used to communicate to and from the gprs shield
Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);  //initiation of the BMP 805 sensor

const int light = 12;            // This is the pin that the leds are connected to in series
Iot firefly;                     // initiation of mini firefly
int PressNum = 0;                // The number of times the button has been pressed
float temperature;               // Variable for BMP
float seaLevelPressure;          // Variable for BMP
float Altitude;                  // Variable for BMP
float pres;                      // Variable for BMP
boolean Timer=false;             // true means start Timer/ false means do not start Timer
const String PhoneBook[] = {("\"YourNumberHere\"") , ("\"YourNumberHere\"")};
void setup(void) 
{
  pinMode(light, OUTPUT);  
  mySerial.begin(19200);               // the GPRS baud rate   
  Serial.begin(9600);                  // The Serial Port baud rate
  firefly= Iot(light);
  
  // Initialise the sensor 
  if(!bmp.begin())
  {
    // There was a problem detecting the BMP085 ... check your connections    
    firefly.sensor_error(0);
    while(1);
  }
  firefly.lcd_firefly_status(0);
  delay(500);
}

void loop(void) 
{ 
  sensors_event_t event;
  bmp.getEvent(&event);
  //uint8_t buttons = firefly.read_button();
  
  if (event.pressure)
  {        
    bmp.getTemperature(&temperature);                   //getTemperature
    seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;    //getPressure
    pres = event.pressure;
    Altitude = bmp.pressureToAltitude(seaLevelPressure, event.pressure, temperature);  //getAltitude

    int num_of_sms = sizeof(PhoneBook)/sizeof(String);                  // Gets the size of the PhoneBook array
    
    for(int i=0; i<num_of_sms; i++) { SendTextMessage(PhoneBook[i]); }  // For loop to send email to everyone in the PhoneBook array
  }
  else {firefly.sensor_error(1);}   //Serial.println("Sensor error"); }

  if(firefly.read_button()==1)// & BUTTON_SELECT)
  {
    if(PressNum%2==0)
    { 
      firefly.on();         // turn the LED on (HIGH is the voltage level)
      firefly.lcd_firefly_status(1);
      delay(3500);
      PressNum=PressNum++;  // adds one to PressNum
      Timer=true;
    }
    else
    {
      firefly.off();        // turn the LED off (LOW is the voltage level)
      firefly.lcd_firefly_status(0);
      PressNum=PressNum++;       
    }  

    // Send sensor data to the Lcd
    firefly.lcd_display_data("Pressure:",event.pressure,"hPa");
    firefly.lcd_display_data("Temperature:",((temperature*9)/5)+32,"F");
    firefly.lcd_display_data("Altitude:",(-1*(bmp.pressureToAltitude(seaLevelPressure, event.pressure, temperature))),"m");
    firefly.lcd_display_data("Voltage Level:",23,"volts");
    
    firefly.lcd_data_status();    
    
    // Send data to the serial monitor on the computer
    /*
    display_in_serial("Temperature",temperature,"C");
    display_in_serial("Pressure",event.pressure,"hPa");
    display_in_serial("Altitude",bmp.pressureToAltitude(seaLevelPressure, event.pressure, temperature),"m");
    */
  }
  Timer = firefly.auto_off(Timer);
}

void display_in_serial(String title, float value, String unit)
{
  Serial.print(title);
  Serial.print(":   ") ;
  Serial.print(value);
  Serial.print(" ");
  Serial.println(unit);  
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
