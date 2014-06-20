/*
  Firefly.h - Library for Firefly Iot demo code.
  Created by David E. Solomon, April 2, 2014. 
*/

#ifndef Firefly_h
#define Firefly_h


#include <Adafruit_RGBLCDShield.h>
#include <Adafruit_MCP23017.h>
#include <inttypes.h>
#include <String.h>
#include <Wire.h>
#include "Print.h"
#include "arduino.h"


class Iot
{
  public:
    Iot();
	Iot(int pin);
	void on();
    void off();
	bool auto_off(bool trigger);
	bool auto_off(bool trigger, int xtimer);
	void lcd_display_data(String title, float value, String unit);
	void lcd_display_data(String title, String value, String unit);	
	bool lcd_firefly_status(int state);
	void lcd_data_status();
	void sensor_error(int num);
	uint8_t read_button();
	String format_time_hr_min_sec(float raw);
  private:
    int _pin;
	void lcd_progress_bar();


};

#endif
