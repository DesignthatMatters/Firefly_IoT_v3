Design That Matters
    Firefly Internet of Things Version 3.0 04/02/2014
	Author: David Solomon 
	Edited by: 

Summary:
	This project uses an Arduino and the Seeedstudio gprs shield to connect a device to a cellular network. Using the Adafruit BMP 180 to collect data and a simple button to
	act as an input. When the button is pressed, the Arduino will collect the data and send it via a text message to an end user.

Bill Of Materials (BoM):
	Link to BoM: https://docs.google.com/a/designthatmatters.org/spreadsheet/ccc?key=0AqZq2xjj9i4ndDhmb3dMM2RPQVU3aS1EeWkzUXc2bFE&usp=sharing
	Arduino software
	Arduino Uno with B usb cable
	Adafruit_sensor library for Arduino 
	Adafruit_BMP085 library for Arduino
	Adafruit_RGB_LCD_Shield library for Arudino
	Electric imp, mini B usb cable and the April board
	Adafruit BMP180 pressure sensor
	lcd shield with keypad for Arduino 
	lcd display
	10k ohm resistors	
	Hook-up wires
	The latest BMP_firefly_v#.ino
	Two 9v batteries and battery packs
	
	
Arduino Set up:
	If you have not already download and install Arduino from http://arduino.cc/en/Main/Software#.UxSScM7tphE
	After, you have installed Arduino, launch the program and add the Adafruit_sensor, Adafruit_BMP085, and Adafruit_RGB_LCD_Shield libraries.
	To add the libraries go to the sketch tab and click on import library -> add library..
	This will open up a dialogue window where you can navigate to where you have saved the libraries that you want to add.
	Select the Adafruit libraries and add them to the sketch. 	

Programming it all:
	Open the Fritzing diagram and set up the circuit.
	Once this is completed open the latest yrmmdd_bfirefly_iot_v#.ino file in Arduino and program the Arduino board.
	
Reference Links:
	BMP180 Barometric Pressure/Temperature/Altitude Sensor 
		http://www.adafruit.com/products/1603#Description

	Adafruit Sensor Arduino library
		https://github.com/adafruit/Adafruit_Sensor
	
	Adafruit BMP085 Arduino library
		https://github.com/adafruit/Adafruit_BMP085_Unified
		
	Sending an email with the electric imp
		http://captain-slow.dk/2014/01/07/using-mailgun-with-electric-imp/
		
	Lcd backpack libraries for Arduino
		https://github.com/adafruit/Adafruit-RGB-LCD-Shield-Library
		
	Seeedstudio gprs shield code example
		http://www.geeetech.com/wiki/index.php/Arduino_GPRS_Shield