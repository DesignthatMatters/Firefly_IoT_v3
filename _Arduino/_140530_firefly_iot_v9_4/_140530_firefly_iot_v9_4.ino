//Include statements
#include <Adafruit_RGBLCDShield.h>
#include <Adafruit_MCP23017.h>
#include <Adafruit_Sensor.h>
#include <Firefly.h>
#include <String.h>
#include <Wire.h>
#include "RTClib.h"
#include <GSM.h>

#define PINNUMBER ""

//Global Variables

const int light = 12;            // This is the pin that the leds are connected to in series
Iot firefly;                     // initiation of mini firefly
int PressNum = 0;                // The number of times the button has been pressed
unsigned long treatmentTime;     // Keeps track of how long the firefly iot has been treating a new bown
boolean Timer=false;             // true means start Timer/ false means do not start Timer
String PhoneBook = "6035925883";  //datawings
char msg[200];
char number[10];

// initialize the library instance
GSM gsmAccess;
GSM_SMS sms;
RTC_Millis rtc;

void setup(void) 
{
  pinMode(light, OUTPUT);  
  rtc.begin(DateTime(F(__DATE__), F(__TIME__)));
  firefly= Iot(light);
   
    // connection state
  boolean notConnected = true;
  
  // Start GSM shield
  // If your SIM has PIN, pass it as a parameter of begin() in quotes
  while(notConnected)
  {
    if(gsmAccess.begin(PINNUMBER)==GSM_READY)
      notConnected = false;
    else
    {
       firefly.lcd_display_data("Connecting","Not finished","");
      delay(1000);
    }
  }
  firefly.lcd_firefly_status(0);
}

void loop(void) 
{   
  if(firefly.read_button()==1)// & BUTTON_SELECT)
  {
    if(PressNum%2==0)
    { 
      firefly.on();         // turn the LED on (HIGH is the voltage level)
      firefly.lcd_firefly_status(1);
      delay(3500);
      PressNum=PressNum++;  // adds one to PressNum
      Send_on_data();
    }
    else
    {
      treatmentTime = millis();
      firefly.off();        // turn the LED off (LOW is the voltage level)
      firefly.lcd_firefly_status(0);
      PressNum=PressNum++;
      Send_off_data(treatmentTime);
    }  
    firefly.lcd_data_status();    
  }
  Timer = firefly.auto_off(Timer);
}

// This function is to send a sms message
void SendTextMessage()//, char data)
{
  sms.beginSMS(number);
  sms.print(msg);
  sms.endSMS();
}

void Send_on_data()
{    
    // Send sensor data to the Lcd
    firefly.lcd_display_data("Pressure:", 1020 , "hPa");
    firefly.lcd_display_data("Temperature:", 22 , "F");
    firefly.lcd_display_data("Altitude:", 200 , "m");
    firefly.lcd_display_data("Voltage Level:", 23, "volts");
    Format_Data_for_datawings();
    SendTextMessage();   // For loop to send email to everyone in the PhoneBook array
}

void Send_off_data(unsigned long treatmentTime)
{
    // Send sensor data to the Lcd
    firefly.lcd_display_data("Pressure:",1020 , "hPa");
    firefly.lcd_display_data("Temperature:",22 , "F");
    firefly.lcd_display_data("Altitude:",200, "m");
    firefly.lcd_display_data("Voltage Level:", 23, "volts");
    firefly.lcd_display_data("Treatment Time:",firefly.format_time_hr_min_sec(treatmentTime),"");
    Format_Data_for_datawings();
    SendTextMessage();   // For loop to send email to everyone in the PhoneBook array
}

void Format_Data_for_datawings()
{    
    char temp[200];
    char temp2[10];
    String mge = "firefly/id 1, sent_at ";
//    String mge = "firefly_id 1, sent_at 2014 .000Z, hour_meter 12, low_temp 15, high_tem 22 ,min_voltage 12, max_voltage 23";
    DateTime now = rtc.now();
    mge += now.year();
    mge +="-";
    mge += now.month();
    mge +="-";
    mge += now.day();
    mge +="T";
    mge += now.hour();
    mge +=":";
    mge += now.minute();
    mge +=":";
    mge += now.second();
    mge += ".000Z, hour_meter 12, low_temp 15,";
    mge += "high_temp 22, min_voltage 12, max";
    mge += "_voltage 23"; 
  
    int size_of = mge.length();
    mge.toCharArray(temp,200);
    for(int i=0; i<(size_of); i++){ msg[i] = temp[i]; }    
    
    int size_of_PH = PhoneBook.length();
    PhoneBook.toCharArray(temp2,10);
    for(int i=0; i<(size_of_PH); i++){ number[i] = temp2[i]; } 
}
