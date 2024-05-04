
/*
  Multiple sensors test with softwareSerial

  In this example the arduino is connected to a ttl to RS485 converter module. 
  To the RS485 converter module multiple Chinese djlk Ultrasonic Sensor is connected. The Serial monitor
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

  // Check if id addresses 0x00 and 0x01 are avilable on the Modbus
  bool checkaddress1 = Ultrasonic_Sensor.CheckAddress(0);
  Serial.print("  check1=" + String (checkaddress1));
  bool checkaddress2 = Ultrasonic_Sensor.CheckAddress(1);
  Serial.print("  check2=" + String (checkaddress2));
  if (checkaddress1 && checkaddress2)
  {
    // First sensor readings
    int addressID = Ultrasonic_Sensor.ReadUltrasonic_Sensor(0x00,ADDRESS_REG);
    Serial.print("  ID=" + String (addressID));
    float RT_distance = Ultrasonic_Sensor.ReadUltrasonic_Sensor(0x00,REALTIME_DISTANCE_REG);
    Serial.print("  RT_distance=" + String (RT_distance));
    float processed_distance = Ultrasonic_Sensor.ReadUltrasonic_Sensor(0x00,PROCESSED_DISTANCE_REG);
    Serial.print("  processed_distance=" + String (processed_distance));
    bool changeaddress = Ultrasonic_Sensor.ChangeAddress(0x00 , 11); // change the id address of Ultrasonic_Sensor from 0x00 to 0x0B
    Serial.print("  CID=" + String (changeaddress));
    RT_distance = Ultrasonic_Sensor.ReadUltrasonic_Sensor(0x0B,REALTIME_DISTANCE_REG);
    Serial.print("  RT_distance=" + String (RT_distance));
    changeaddress = Ultrasonic_Sensor.ChangeAddress(11 , 0x00);  // change the id address of Ultrasonic_Sensor from 0x0B to 0x00
    Serial.print("  CID=" + String (changeaddress));

    // Second sensor
    addressID = Ultrasonic_Sensor.ReadUltrasonic_Sensor(0x01,ADDRESS_REG);
    Serial.print("  ID=" + String (addressID));
    RT_distance = Ultrasonic_Sensor.ReadUltrasonic_Sensor(0x01,REALTIME_DISTANCE_REG);
    Serial.print("  RT_distance=" + String (RT_distance));
    processed_distance = Ultrasonic_Sensor.ReadUltrasonic_Sensor(0x01,PROCESSED_DISTANCE_REG);
    Serial.print("  processed_distance=" + String (processed_distance));
    changeaddress = Ultrasonic_Sensor.ChangeAddress(0x01 , 11); // change the id address of Ultrasonic_Sensor from 0x01 to 0x0B
    Serial.print("  CID=" + String (changeaddress));
    RT_distance = Ultrasonic_Sensor.ReadUltrasonic_Sensor(0x0B,REALTIME_DISTANCE_REG);
    Serial.print("  RT_distance=" + String (RT_distance));
    changeaddress = Ultrasonic_Sensor.ChangeAddress(11 , 0x01);  // change the id address of Ultrasonic_Sensor from 0x0B to 0x01
    Serial.println("  CID=" + String (changeaddress));
  }
  else
  {
    Serial.println("Check if both sensors are connected correctly on the Modbus");
  }
}
