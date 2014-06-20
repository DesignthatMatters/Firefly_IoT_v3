/*
  Firefly.cpp - Library for Firefly Iot demo code.
  Created by David E. Solomon, April 2, 2014. 
*/

#include "arduino.h"
#include "Firefly.h"
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <Adafruit_RGBLCDShield.h>
#include <Adafruit_MCP23017.h>
#include <Wire.h>

int xTimer=180;                // This is the counter for the timer to indicate how many 500ms delays has occurred
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();


Iot::Iot(){}

Iot::Iot(int pin)
{  
  pinMode(pin, OUTPUT);
  lcd.begin(16, 2);
  _pin = pin;
}

void Iot::on()
{
  digitalWrite(_pin, HIGH);
  delay(250);
}

void Iot::off()
{
  digitalWrite(_pin, LOW);
  delay(250);
}

void Iot::lcd_display_data(String title, float value, String unit)
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

void Iot::lcd_display_data(String title, String value, String unit)
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

void Iot::lcd_data_status()
{	
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Sending Report");
  lcd.setCursor(0,1);
  lcd.print("About Firefly! ");
  delay(3500);
  lcd_progress_bar();
  lcd.clear();
  lcd.setCursor(0,0);

  lcd.print("Report About");
  lcd.setCursor(0,1);
  lcd.print("Firefly was sent!");  
  delay(3500);
  lcd.clear();
  lcd.setBacklight(0x0);
}

void Iot::lcd_progress_bar()
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

bool Iot::lcd_firefly_status(int state)
{
  switch(state)
  {
	case 1:
		lcd.clear();
		lcd.setBacklight(0x7);
		lcd.setCursor(1,0);
		lcd.print("Firefly is on");
		lcd.setCursor(4,1);
		lcd.print(" at DtM");
		delay(3500);
		return true;
	break;
	case 0:
		lcd.clear();
		lcd.setBacklight(0x7);
		lcd.setCursor(1,0);
		lcd.print("Firefly is off");
		lcd.setCursor(4,1);
		lcd.print("at DtM ");  
		delay(3500);
		lcd.setBacklight(0x0);
		return false; 
	break;
	default:
		lcd.clear();
		lcd.setBacklight(0x7);
		lcd.setCursor(0,0);
		lcd.print("Firefly state is");
		lcd.setCursor(4,1);
		lcd.print("unknown");
		delay(3500);
		return false;
	break;
  }
}

bool Iot::auto_off(bool trigger)
{   
  if(trigger)
  {
      delay(500);
      if(xTimer==0)
      { 
         digitalWrite(_pin, LOW); 
         xTimer=0;
         trigger=false;
		 return trigger;
      }
      else 
          xTimer--;       
  }
  else
  {
	delay(500);
  }
}

void Iot::sensor_error(int num)
{
	switch(num)
	{
		case 0:
			lcd.clear();
			lcd.setBacklight(0x7);             // Turns on the backlight of the lcd screen
			lcd.setCursor(0,0);                // The place where you want the first word to begin setCursor(PLACE,ROW)
			lcd.print("Sensors were not");
			lcd.setCursor(5,1);
			lcd.print("Found");
		break;
		case 1:
			lcd.clear(); lcd.setCursor(0,0); lcd.print("Sensor error");
		break;
		default:		
		break;

	}
}

uint8_t Iot::read_button()
{
	uint8_t b= lcd.readButtons();
	return b;
}

String Iot::format_time_hr_min_sec(float raw)
{
    unsigned long temp = raw;
    String Time_formatted;
    int hour=0;
    int minute=0;
    int seconds=0;
    if(60>(temp/(1000))&&(temp/(1000))>=1)
    {
        temp = ((temp/1000)+ 4);        
        Time_formatted = (String)temp;        
        Time_formatted += " Sec(s)";
    }
    else if(60>(temp/(60000))&&(temp/(60000))>=1)
    {
        temp = temp/(1000);
        Time_formatted = (String)(temp/60);
        Time_formatted += " Min(s) " ;
        Time_formatted += (String)((temp%60)+ 4) ;
        Time_formatted += " Sec(s)";
    }
    else if(24>(temp/(3600000))&&(temp/(3600000))>=1)
    {
        temp = temp/(1000);
        Time_formatted = (String)(temp/3600);
        Time_formatted += " Hr(s) ";
        Time_formatted += (String)((temp/60)%60); 
        Time_formatted += " Min(s) ";
    }
    return Time_formatted;
}