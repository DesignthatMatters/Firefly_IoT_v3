#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>
#include <Adafruit_MCP23017.h>
#include <Adafruit_RGBLCDShield.h>

Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();
Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);

const int light = 12;            // This is the pin that the leds are connected to in series
int PressNum = 0;                // The number of times the button has been pressed
int xTimer=0;                    // This is the counter for the timer to indicate how many 500ms delays has occurred
const int indicator = 7;         // This tells the imp that the button was pressed

float temperature;
float seaLevelPressure;
float Altitude;
float pres;
boolean Timer=false;

void setup(void) 
{
  pinMode(light, OUTPUT);
  pinMode(indicator, OUTPUT);
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.setBacklight(0x0);
 
  // Initialise the sensor 
  if(!bmp.begin())
  {
    // There was a problem detecting the BMP085 ... check your connections    
    lcd.setBacklight(0x7);
    lcd.setCursor(0,0);
    lcd.print("Sensors were not");
    lcd.setCursor(0,1);
    lcd.print("Found");
    while(1);
  }
  FFoff();
}

void loop(void) 
{
  sensors_event_t event;
  bmp.getEvent(&event);
  uint8_t buttons = lcd.readButtons();
  
  if (event.pressure)
  {        
    bmp.getTemperature(&temperature); //getTemperature
    seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;
    Altitude = bmp.pressureToAltitude(seaLevelPressure, event.pressure, temperature);
    pres = event.pressure;

    // Send sensor data to the imp
    byte data[3];
    pres=pres/5;
    data[0]= (byte)(temperature);// + Altitude + pres);
    data[1] = (byte)(Altitude);
    data[2] = (byte)(pres);
    Serial.write(data,3);
  }
  else { lcd.clear(); lcd.setCursor(0,0); lcd.print("Sensor error");} //Serial.println("Sensor error"); }

  if(buttons & BUTTON_SELECT)
  {
    digitalWrite(indicator,HIGH);
    if(PressNum%2==0)
    { 
      digitalWrite(light, HIGH);   // turn the LED on (HIGH is the voltage level)
      FFon();
      delay(3500);
      PressNum=PressNum++;
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
    digitalWrite(indicator,LOW);
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
  {
    delay(500);
  }
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
