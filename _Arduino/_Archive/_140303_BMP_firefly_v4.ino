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

Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);
int led = 8;
const int buttonPin = 7;
int buttonState;             // the current reading from the input pin

void setup(void) 
{  
  pinMode(buttonPin, INPUT);
  pinMode(led, OUTPUT); 
  Serial.begin(9600);

  /* Initialise the sensor */
  if(!bmp.begin())
  {
    /* There was a problem detecting the BMP085 ... check your connections */
    // Serial.print("Ooops, no BMP085 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
}

void loop(void) 
{   
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {
    digitalWrite(led, LOW);   // turn the LED on (HIGH is the voltage level)

    sensors_event_t event;
    bmp.getEvent(&event);

    /* Display the results (barometric pressure is measure in hPa) */
    if (event.pressure)
    {
      /* Display atmospheric pressue in hPa */
      /*Serial.print("Pressure:    ");
       Serial.print(event.pressure);
       Serial.println(" hPa");*/
      float pressure =event.pressure;     

      /* First we get the current temperature from the BMP085 */
      float temperature;
      bmp.getTemperature(&temperature);
      //Serial.print("Temperature: ");
      //Serial.print(" C     ");
      float tempc=temperature;

      float seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;
      /*Serial.print("Altitude:    "); 
       Serial.print(bmp.pressureToAltitude(seaLevelPressure,
       event.pressure,
       temperature)); 
       Serial.println(" m");
       Serial.println("");*/

      float altitude =bmp.pressureToAltitude(seaLevelPressure, event.pressure,temperature);  
      byte data[] = {(byte)tempc,(byte)pressure,(byte)altitude};

      Serial.write(data, 4);
      delay(4000);
    }   
    else { Serial.println("Sensor error"); }  
  }
  else{}
  
  
  digitalWrite(led, HIGH);    // turn the LED off by making the voltage LOW
  
  }





