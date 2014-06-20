#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>
#include <Adafruit_MCP23017.h>
#include <Adafruit_RGBLCDShield.h>

/* This driver uses the Adafruit unified sensor library (Adafruit_Sensor),
   which provides a common 'type' for sensor data and some helper functions.
   
   To use this driver you will also need to download the Adafruit_Sensor
   library and include it in your libraries folder.

   You should also assign a unique ID to this sensor for use with
   the Adafruit Sensor API so that you can identify this particular
   sensor in any data logs, etc.  To assign a unique ID, simply
   provide an appropriate value in the constructor below (12345
   is used by default in this example).
   
   Connections
   ===========
   Connect SCL to analog 5
   Connect SDA to analog 4
   Connect VDD to 3.3V DC
   Connect GROUND to common ground
    
   History
   =======
   2013/JUN/17  - Updated altitude calculations (KTOWN)
   2013/FEB/13  - First version (KTOWN)
*/
   
Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);

Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();
//int led = 13;            // led pin number

/**************************************************************************/
/*
    Displays some basic information on this sensor from the unified
    sensor API sensor_t type (see Adafruit_Sensor for more information)
*/
/**************************************************************************/
void displaySensorDetails(void)
{
  sensor_t sensor;
  bmp.getSensor(&sensor);
  /*Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" hPa");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" hPa");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" hPa");  
  Serial.println("------------------------------------");
  Serial.println("");*/
  delay(500);
}

/**************************************************************************/
/*
    Arduino setup function (automatically called at startup)
*/
/**************************************************************************/
void setup(void) 
{
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.print("Pressure Sensor"); 
  Serial.println("Pressure Sensor Test"); Serial.println("");
  
  /* Initialise the sensor */
  if(!bmp.begin())
  {
    /* There was a problem detecting the BMP085 ... check your connections */
    Serial.print("Ooops, no BMP085 detected ... Check your wiring or I2C ADDR!");
    lcd.setBacklight(0x7);
    lcd.setCursor(0,1);
    lcd.print("no BMP085 detected");
    while(1);
  }
  
  /* Display some basic information on this sensor */
  displaySensorDetails();
  
  //pinMode(led, OUTPUT);  // configure LED pin
}

/**************************************************************************/
/*
    Arduino loop function, called once 'setup' is complete (your own code
    should go here)
*/
/**************************************************************************/
void loop(void) 
{
 /* int b = 0;
  // if there's data available
  if (Serial.available () > 0) {
    // read a byte
    b = Serial.read();
    if (b == 0x00) {
      digitalWrite(led, LOW);
      Serial.write(0x10);
    } else if (b == 0x01) {
      digitalWrite(led, HIGH);
      Serial.write(0x11);
    }
  }*/
  
  /* Get a new sensor event */ 
  sensors_event_t event;
  bmp.getEvent(&event);
  uint8_t buttons = lcd.readButtons();
 
  /* Display the results (barometric pressure is measure in hPa) */
  if (event.pressure)
  {
    /* Display atmospheric pressue in hPa */
    Serial.print("Pressure:    ");
    Serial.print(event.pressure);
    Serial.println(" hPa");
    display_on_lcd("Pressure:",event.pressure,"hPa");
 
     /* Calculating altitude with reasonable accuracy requires pressure    *
     * sea level pressure for your position at the moment the data is     *
     * converted, as well as the ambient temperature in degress           *
     * celcius.  If you don't have these values, a 'generic' value of     *
     * 1013.25 hPa can be used (defined as SENSORS_PRESSURE_SEALEVELHPA   *
     * in sensors.h), but this isn't ideal and will give variable         *
     * results from one day to the next.                                  *
     *                                                                    *
     * You can usually find the current SLP value by looking at weather   *
     * websites or from environmental information centers near any major  *
     * airport.                                                           *
     *                                                                    *
     * For example, for Paris, France you can check the current mean      *
     * pressure and sea level at: http://bit.ly/16Au8ol                   */
     
    /* First we get the current temperature from the BMP085 */
    float temperature;
    bmp.getTemperature(&temperature);
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print(" C     ");
    Serial.print(((temperature*9)/5)+32);
    Serial.println(" F");
    
    delay(1000);
    display_on_lcd("Temperature:",temperature,"C");
    
    

    /* Then convert the atmospheric pressure, SLP and temp to altitude    */
    /* Update this next line with the current SLP for better results      */
    float seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;
    Serial.print("Altitude:    "); 
    Serial.print(bmp.pressureToAltitude(seaLevelPressure,
                                        event.pressure,
                                        temperature)); 
    Serial.println(" m");
    Serial.println("");

    delay(1000);
    display_on_lcd("Altitude:",bmp.pressureToAltitude(seaLevelPressure, event.pressure, temperature),"m");
    
    if (buttons & BUTTON_SELECT) 
    {
      lcd.clear();
      lcd.setCursor(0,0); 
      lcd.print("Sending Data ");
      delay(10000);     
    }   
       
  }
  else
  {
    Serial.println("Sensor error");
  }
  delay(1000);
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
