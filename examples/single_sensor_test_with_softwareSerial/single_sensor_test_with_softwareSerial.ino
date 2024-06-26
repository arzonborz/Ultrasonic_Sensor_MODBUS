/*
  Single sensor test with softwareSerial

  In this example the arduino is connected to a ttl to RS485 converter module. 
  To the RS485 converter module a Chinese djlk Ultrasonic Sensor is connected. The Serial monitor
  is used to view the date using softwareSerial.

  The circuit:
  * Pin 10, 11 are connected to RX and TX of the ttl to RS485 converter respectively

  Created 27/4/2024
  By Ghaith Alshishani

*/
#include <SoftwareSerial.h> 
SoftwareSerial mySerial(10, 11); // RX, TX

#include "Ultrasonic_Sensor_MODBUS.h"
Ultrasonic_Sensor_MODBUS Ultrasonic_Sensor(&mySerial); // create an Ultrasonic_Sensor_MODBUS on Serial port (i.e: 0(RX),1(TX) pins or hardware Serial1) 

void setup() {
  // initialize serial transmission with NO parity
  Serial.begin(9600,SERIAL_8N1);
  // initialize serial transmission using softwareSerial
  mySerial.begin(9600);
}

void loop() {
  // ReadAddress read and retuns the ID address of the Ultrasonic_Sensor on the RS485 MODBUS. There should only be one Ultrasonic_Sensor connected to the bus at a time because this function uses the broadcast ID address
  int readaddress = Ultrasonic_Sensor.ReadAddress();
  Serial.print("readaddress=" + String (readaddress));
  // SetAddress returns a boolean to determine succeful operation. This function Sets the ID address of the Ultrasonic_Sensor on the RS485 MODBUS. There should only be one Ultrasonic_Sensor connected to the bus at a time because this function uses the broadcast ID address
  bool setaddress = Ultrasonic_Sensor.SetAddress(0);
  Serial.print("  setaddress=" + String (setaddress));
  // CheckAddress returns a boolean and checks if the Sensor ID is present on RS485 MODBUS
  bool checkaddress = Ultrasonic_Sensor.CheckAddress(0);
  Serial.print("  checkaddress=" + String (checkaddress));
  // reads the data registers from Ultrasonic_Sensors with a specified id. returns (-255) when wrong id is passed (-254) when wrong address is passed (-253 & -252) when no correct bytes were recieved
  int addressID = Ultrasonic_Sensor.ReadUltrasonic_Sensor(0x00,ADDRESS_REG);
  Serial.print("  addressID=" + String (addressID));
  float REALTIME_DISTANCE = Ultrasonic_Sensor.ReadUltrasonic_Sensor(0x00,REALTIME_DISTANCE_REG);
  Serial.print("  REALTIME_DISTANCE=" + String (REALTIME_DISTANCE));
  float PROCESSED_DISTANCE = Ultrasonic_Sensor.ReadUltrasonic_Sensor(0x00,PROCESSED_DISTANCE_REG);
  Serial.print("  PROCESSED_DISTANCE=" + String (PROCESSED_DISTANCE));
  // ChangeAddress changes the id address of a given Ultrasonic_Sensor id address if the new address is not already taken on the RS485 MODBUS
  bool changeaddress = Ultrasonic_Sensor.ChangeAddress(0x00 , 11);
  Serial.print("  changeaddress=" + String (changeaddress));
  REALTIME_DISTANCE = Ultrasonic_Sensor.ReadUltrasonic_Sensor(0x0B,REALTIME_DISTANCE_REG);
  Serial.println("  REALTIME_DISTANCE=" + String (REALTIME_DISTANCE));
}