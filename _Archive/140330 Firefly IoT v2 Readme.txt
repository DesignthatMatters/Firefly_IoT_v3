Design That Matters
    Firefly Internet of Things Version 2.0 03/30/2014
	Author: David Solomon 
	Edited by: 

Summary:
	This project uses an Arduino, and an electric imp to connect a device to the internet. Using the Adafruit BMP 180 to collect data and a simple button to
	act as an input. when the button is pressed, The imp will gather data from the Arduino. Once the data is on the imp a function is called that will send 
	an email or sms message with the information that was collected from the BMP180 sensor to a specific user.

Bill Of Materials (BoM):
	Link to BoM: https://docs.google.com/a/designthatmatters.org/spreadsheet/ccc?key=0AqZq2xjj9i4ndDhmb3dMM2RPQVU3aS1EeWkzUXc2bFE&usp=sharing
	Arduino software
	Arduino Uno with B usb cable
	Adafruit_sensor library for Arduino 
	Adafruit_BMP085 library for Arduino
	Adafruit_MCP23017 library for Arduino
	Adafruit_RGBLCDShelid library for Arudino
	Electric imp, mini B usb cable and the April board
	Adafruit BMP180 pressure sensor
	lcd shield with keypad for Arduino 
	lcd display
	10k ohm resistors	
	Hook-up wires
	A smart device with Android or ios(phone or tablet) with the electric imp mobile application
	The latest BMP_firefly_v#.ino
	The latest Firefly IoT agent.txt
	The latest Firefly IoT devices.txt


	Imp Set up:

	If you have not already go to https://electricimp.com/ and create an account.
		This allows you to program the electric imp. 
	After you have create an account the next step is to add the imp to your wireless internet
	Using you smart device change the display settings on your device. It is recommended that 
	you have your brightness up all the way and you turn off automatic adjust screen brightness. 
	Also, make sure you power management is off for this process.
	Before you sync the imp to your wireless network. Make sure your smart device is connected to the wireless
	network that you want your imp to join.
	When this is completed open the electric imp application and sign into your account.
	You should see the wireless networks that your smart device is connected to select that network.
		If you network is not there you should be able to add it in manually.
	Place the imp chip inside the April board. Make sure that the jumper on April board is placed on "USB" or "BAT"
	Now to connect the imp with the network you can plug the April board into the usb port on the computer or power 
	it with a battery pack using the Vin and Gnd pins on the April board
	It should start up by blinking orange, now hold the April board flush to the smart device and press "Send BlinkUp"
	The screen will start to blink in about 3 seconds and it should be connecting the imp to the network
	The smart device will stop blinking after a while and the imp will start flashing red and orange for a few seconds.
	You will know that it is connected when  you see a green light appears on the imp. 

	Reference: https://electricimp.com/docs/gettingstarted/1-blinkup/
			   https://electricimp.com/docs/troubleshooting/blinkup/	

Arduino Set up:
	If you have not already download and install Arduino from http://arduino.cc/en/Main/Software#.UxSScM7tphE
	After, you have installed Arduino, launch the program and add the Adafruit_sensor and Adafruit_BMP085 libraries.
	To add the libraries go to the sketch tab and click on import library -> add library..
	This will open up a dialogue window where you can navigate to where you have saved the libraries that you want to add.
	Select the Adafruit libraries and add them to the sketch. 	

Programming it all:
	Open the Fritzing diagram and set up the circuit.
	Once this is completed open the latest BMP_firefly.ino file in Arduino and program the Arduino board.
		IMPORTANT: Make sure you disconnect anything connected to digital pin 0 and 1 on the Arduino board as this will cause 
					failures in the uploading of the code to the board.
	After, the code is uploaded reconnect the cables to pin 0 and 1.
	The next step is programming the imp. 
	Open a web browser and go https://electricimp.com/ and sign in. This will bring you to programming environment. 
	If this is the first time logging in or you are using a different imp you will see under unassigned devices your imp. 
	Hoover over it and move the mouse to the right side and click on the symbol that looks like an gear.
	Here you can change the name of the imp so it is easier to recognize although it is not required
	You can also create or associate the imp with a model, a model is where all the code is placed that will be programmed to the imp.
	Create a new model called Firefly IoT.
	Now, Open Firefly IoT agent in notepad and copy the code. Place that code in the agent section of the model
		In the agent code there is a variable called "to", Within the brackets place the email address that you want to receive the information with
	Now, Open Firefly IoT device in notepad and copy the code. Place that code in the device section of the model
	Now if your imp is on click on build and run. In the log window it will show you the status of the imp. 
	
Reference Links:
	BMP180 Barometric Pressure/Temperature/Altitude Sensor 
		http://www.adafruit.com/products/1603#Description
	
	Adafruit Sensor Arduino library
		https://github.com/adafruit/Adafruit_Sensor
	
	Adafruit BMP085 Arduino library
		https://github.com/adafruit/Adafruit_BMP085_Unified
		
	Sending an email with the electric imp
		http://captain-slow.dk/2014/01/07/using-mailgun-with-electric-imp/
		
	Communication between the imp and the Arduino board
		http://electricimp.com/docs/api/hardware/uart/
		
	Usefully information about programming the electric imp
		http://www.electricimp.com/docs/api/
		
	Lcd backpack libraries for Arduino
		https://github.com/adafruit/Adafruit-RGB-LCD-Shield-Library