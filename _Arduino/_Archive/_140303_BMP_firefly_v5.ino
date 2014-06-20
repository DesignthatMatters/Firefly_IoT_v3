#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>

/*    
 Connections
 ===========
 Connect SCL to analog 5
 Connect SDA to analog 4
 Connect VDD to 3.3V DC
 Connect GROUND to common ground   

 Summary
 ========
 This Arduino code calculates the temperture, pressure, and altitude of the area and 
 sends that information over to the electric imp using the tx port (digital pin 1) on the arduino 
 to the rx port on the imp (pin 5) and serial.write
 */

const int led = 12;            // This is the pin that the leds are connected to in series
const int buttonPin = 8;       /* 
                                 This is the pin where the output of the button connects to the board
                                 Connect one pin of the button to ground and on the same side 
                                 connect the other pin to 3.3v
                                 on the other side connect the pin accross from ground to the pin 
                                 defined in this experiment (Pin 7)
                               */
int buttonState=0;             // The current reading from the input pin
int PressNum = 0;              // The number of times the button has been pressed

Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);

void setup(void) 
{  
  pinMode(buttonPin, INPUT);
  pinMode(led, OUTPUT); 
  Serial.begin(9600);

  /* Initialise the sensor */
  if(!bmp.begin())
  {
    /* There was a problem detecting the BMP085 ... check your connections */
    Serial.println("Ooops, no BMP085 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
}

void loop(void) 
{   
  sensors_event_t event;
  bmp.getEvent(&event);
    
  buttonState = digitalRead(buttonPin); 
  if (buttonState == HIGH) {
    if(PressNum%2==0)
    {
          digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
          PressNum=PressNum++;
          delay(2000);  
    }
    else
    {
         digitalWrite(led, LOW);   // turn the LED off(HIGH is the voltage level)
         PressNum=PressNum++;
         delay(2000);  
    }
  
  
    

    /* Display the results (barometric pressure is measure in hPa) */
    if (event.pressure)
    {
      /* Display atmospheric pressue in hPa */
      /*
         Serial.print("Pressure:    ");
         Serial.print(event.pressure);
         Serial.println(" hPa");    
      */ 
        float pressure = event.pressure;     
         
      /* First we get the current temperature from the BMP085 */
      float temperature;
      bmp.getTemperature(&temperature);
      float tempc=temperature;
      /*
        Serial.print("Temperature: ");
        Serial.print(tempc);
        Serial.println(" C     ");
      */
      

      float seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;
      /*
         Serial.print("Altitude:    "); 
         Serial.print(bmp.pressureToAltitude(seaLevelPressure, event.pressure, temperature)); 
         Serial.println(" m");
         Serial.println("");
       */
       
      //pressure = pressure/4;
      float altitude = bmp.pressureToAltitude(seaLevelPressure, event.pressure,temperature);  
      byte data[] = {(byte)tempc,(byte)pressure,(byte)altitude};

      Serial.write(data,4);
    }   
    else { Serial.println("Sensor error"); }
  }
}
  





