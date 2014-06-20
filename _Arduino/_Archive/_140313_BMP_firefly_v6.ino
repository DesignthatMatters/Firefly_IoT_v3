#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>
   
Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);

const int led = 12;            // This is the pin that the leds are connected to in series
const int buttonPin = 8;       /* 
                                 This is the pin where the output of the button connects to the board
                                 Connect one pin of the button to ground and on the same side 
                                 connect the other pin to 3.3v
                                 on the other side connect the pin accross from ground to the pin 
                                 defined in this experiment (Pin 7)
                               */
int PressNum = 0;              // The number of times the button has been pressed                             
const int indicator = 7;

void setup(void) 
{
  pinMode(buttonPin, INPUT);
  pinMode(led, OUTPUT);
  pinMode(indicator, OUTPUT);
  Serial.begin(9600);
  //Serial.println("Pressure Sensor Test"); Serial.println("");
  
  // Initialise the sensor 
  if(!bmp.begin())
  {
    // There was a problem detecting the BMP085 ... check your connections 
    Serial.print("Ooops, no BMP085 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
}

void loop(void) 
{
  sensors_event_t event;
  bmp.getEvent(&event);
  int buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) 
  {
    digitalWrite(indicator,HIGH);
    if(PressNum%2==0)
    { 
      digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
      PressNum=PressNum++;
    }
    else
    {
      digitalWrite(led, LOW);   // turn the LED off(HIGH is the voltage level)
      PressNum=PressNum++;       
    }  
    
    if (event.pressure)
    {    
      float temperature;
      bmp.getTemperature(&temperature);
      /*
      Serial.print("Temperature: ");    
      Serial.print(temperature);
      Serial.println(" C     ");
      */
      
      float seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;
      /*
      Serial.print("Altitude:    "); 
      Serial.print(bmp.pressureToAltitude(seaLevelPressure,
                                          event.pressure,
                                          temperature)); 
      Serial.println(" m");
      Serial.println("");
      */
      float Altitude = bmp.pressureToAltitude(seaLevelPressure, event.pressure, temperature);
      float pres = event.pressure;
      digitalWrite(indicator,LOW); 
      char data = (char)(temperature + Altitude + pres);
      Serial.write(data);
    }
    else
    {
      Serial.println("Sensor error");
    }
    delay(500);
  }
}
