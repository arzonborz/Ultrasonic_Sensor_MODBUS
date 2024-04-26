/*
  Single sensor test with softwareSerial

  In this example the arduino is connected to a ttl to RS485 converter module. 
  To the RS485 converter module a Chinese djlk Ultrasonic Sensor is connected. A ttl to 
  usb FTDI converter is used to view the date using softwareSerial.

  The circuit:
  * Pin 10, 11 are connected TX, RX of the FTDI respectively
  * Pin 0(RX),1(TX) are connected to RX and TX of the ttl to RS485 converter
    respectively

  Created 27/4/2024
  By Ghaith Alshishani

*/

#include "Ultrasonic_Sensor_MODBUS.h"

Ultrasonic_Sensor_MODBUS Ultrasonic_Sensor(&Serial); // create an Ultrasonic_Sensor_MODBUS on Serial port (i.e: 0(RX),1(TX) pins or hardware Serial1) 

#include <SoftwareSerial.h> 
SoftwareSerial mySerial(10, 11); // RX, TX


void setup() {
  // initialize serial transmission with NO parity
  Serial.begin(9600,SERIAL_8N1);
  // initialize serial transmission using softwareSerial
  mySerial.begin(9600);
}

void loop() {
  // ReadAddress read and retuns the ID address of the Ultrasonic_Sensor on the RS485 MODBUS. There should only be one Ultrasonic_Sensor connected to the bus at a time because this function uses the broadcast ID address
  int readaddress = Ultrasonic_Sensor.ReadAddress();
  mySerial.print("readaddress=" + String (readaddress));
  // SetAddress returns a boolean to determine succeful operation. This function Sets the ID address of the Ultrasonic_Sensor on the RS485 MODBUS. There should only be one Ultrasonic_Sensor connected to the bus at a time because this function uses the broadcast ID address
  bool setaddress = Ultrasonic_Sensor.SetAddress(0);
  mySerial.print("  setaddress=" + String (setaddress));
  // CheckAddress returns a boolean and checks if the Sensor ID is present on RS485 MODBUS
  bool checkaddress = Ultrasonic_Sensor.CheckAddress(0);
  mySerial.print("  checkaddress=" + String (checkaddress));
  // reads the data registers from Ultrasonic_Sensors with a specified id. returns (-255) when wrong id is passed (-254) when wrong address is passed (-253 & -252) when no correct bytes were recieved
  int addressID = Ultrasonic_Sensor.ReadUltrasonic_Sensor(0x00,ADDRESS_REG);
  mySerial.print("  addressID=" + String (addressID));
  float REALTIME_DISTANCE = Ultrasonic_Sensor.ReadUltrasonic_Sensor(0x00,REALTIME_DISTANCE_REG);
  mySerial.print("  temp=" + String (REALTIME_DISTANCE));
  float PROCESSED_DISTANCE = Ultrasonic_Sensor.ReadUltrasonic_Sensor(0x00,PROCESSED_DISTANCE_REG);
  mySerial.print("  waterContent=" + String (PROCESSED_DISTANCE));
  // ChangeAddress changes the id address of a given Ultrasonic_Sensor id address if the new address is not already taken on the RS485 MODBUS
  bool changeaddress = Ultrasonic_Sensor.ChangeAddress(0x00 , 11);
  mySerial.print("  changeaddress=" + String (changeaddress));
  REALTIME_DISTANCE = Ultrasonic_Sensor.ReadUltrasonic_Sensor(0x0B,REALTIME_DISTANCE_REG);
  mySerial.println("  counts=" + String (REALTIME_DISTANCE));
}
