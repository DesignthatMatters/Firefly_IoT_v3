#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>
#include <Adafruit_MCP23017.h>
#include <Adafruit_RGBLCDShield.h>

Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();
Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);

const int light = 12;            // This is the pin that the leds are connected to in series
int PressNum = 0;               // The number of times the button has been pressed                             
const int indicator = 7;


void setup(void) 
{
  pinMode(light, OUTPUT);
  pinMode(indicator, OUTPUT);
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.setBacklight(0x0);
  //lcd.print("Pressure Sensor");
  //Serial.println("Pressure Sensor Test"); Serial.println("");
  
  // Initialise the sensor 
  if(!bmp.begin())
  {
    // There was a problem detecting the BMP085 ... check your connections 
    //Serial.print("Ooops, no BMP085 detected ... Check your wiring or I2C ADDR!");
    lcd.setBacklight(0x7);
    lcd.setCursor(0,0);
    lcd.print("Sensors were not");
    lcd.setCursor(0,1);
    lcd.print("Found");
    while(1);
  }
}

void loop(void) 
{
  sensors_event_t event;
  bmp.getEvent(&event);
  uint8_t buttons = lcd.readButtons();
  //int buttonState = digitalRead(buttonPin);
  //if (buttonState == HIGH) 
  if(buttons & BUTTON_SELECT)
  {
    digitalWrite(indicator,HIGH);
    if(PressNum%2==0)
    { 
      digitalWrite(light, HIGH);   // turn the LED on (HIGH is the voltage level)
      PressNum=PressNum++;
    }
    else
    {
      digitalWrite(light, LOW);   // turn the LED off(HIGH is the voltage level)
      PressNum=PressNum++;       
    }  
    
    if (event.pressure)
    {        
      float temperature;
      float seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;
      float Altitude = bmp.pressureToAltitude(seaLevelPressure, event.pressure, temperature);
      float pres = event.pressure;

// Send sensor data to the imp
      byte data[3] ={};
      data[0]= (byte)(temperature);// + Altitude + pres);
      data[1] = (byte)(Altitude);
      data[2] = (byte)(pres);
      Serial.write(data,3);

// Send sensor data to the Lcd
      display_on_lcd("Pressure:",event.pressure,"hPa");      
      bmp.getTemperature(&temperature); //getTemperature
      delay(1000);
      display_on_lcd("Temperature:",((temperature*9)/5)+32,"C");
      delay(1000);
      display_on_lcd("Altitude:",bmp.pressureToAltitude(seaLevelPressure, event.pressure, temperature),"m");
      delay(1000);
      lcd.clear();
      lcd.setBacklight(0x0);
      
// Send data to the serial monitor on the computer
      /*
      Serial.print("Temperature: ");    
      Serial.print(temperature);
      Serial.println(" C     ");
      
      Serial.print("Altitude:    "); 
      Serial.print(bmp.pressureToAltitude(seaLevelPressure, event.pressure, temperature)); 
      Serial.println(" m");
      Serial.println("");
      */
      digitalWrite(indicator,LOW);
    }
    else
    {
      Serial.println("Sensor error");
    }
    delay(500);
  }
}

void display_on_lcd(String title, float value, String unit)
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
}
