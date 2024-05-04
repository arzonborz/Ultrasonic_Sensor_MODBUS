# Ultrasonic_Sensor_MODBUS

![Version](https://img.shields.io/badge/Version-v1.0.0-green.svg)

*** IMPORTANT : The djlk ultrasonic module i tested proved to be utterly useless due to inconsistent readings. This library is just for documentation in case someone wants to try this cheap module without going through chinese documentations ***

***Please Note: This is a personal effort and the product manufacturer is not related nor responsible for using or supporting this library***

![image](https://github.com/arzonborz/Ultrasonic_Sensor_MODBUS/assets/61315293/a915c304-4a2f-415a-b956-8cb1793dddfd)

 Library for communicating through RS-485 Modbus with a Chinese djlk ultrasonic sensor using the Arduino platform.

### Features
---
* RS-485 Modbus broadcast functionality
* Communication with several sensors on the same RS-485 Modbus
* Set address ID of djlk ultrasonic sensor
* Change address ID of a specific djlk ultrasonic sensor on-the-fly (even with multiple sensors connected on the same RS-485 Modbus)
* Address collision protection before setting/changing address ID
* Read the data registers of djlk ultrasonic sensor, which are:
  * Address ID
  * real-time distance value
  * processed distance value
* Check if an address ID is present on RS-485 Modbus

### Future changes
---
* None

### Commands
---
Ultrasonic_Sensor_MODBUS(Stream *port);

int ReadAddress();

bool SetAddress(byte newId);

bool CheckAddress(byte id);

bool ChangeAddress(byte oldID , byte newId);

float ReadUltrasonic_Sensor(byte id, String reg);

bool RS485_TRANSEIVER(String msgSent , byte numOfSentByte, byte* msgReceived, byte numOfReceivedByte, int receiveTimeout);

### Usage
---
The library is not yet included in the IDE/IO library manager. Alternatively, one can simply clone this library to your working library folder and include "SMT100_MODBUS.h" in his main sketch.

If you are having difficulties, please raise an issue.

### A Bit About this Sensor
---
See extras for the djlk ultrasonic sensor documentation.

### Authors
---
Ghaith Alshishani, you can find my contact details below.

### License
---
This project is licensed under Apache 2.0 - see the LICENSE.md file for details

### Feedback
---
For any feedback contact me on my Email: arzonborz@gmail.com
