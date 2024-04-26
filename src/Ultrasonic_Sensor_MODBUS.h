/****************************************************** 
  Arduino library for communicating though RS-485 Modbus with chinese djlk ultrasonic sensor
  
  Author: Ghaith alshishani arzonborz@gmail.com
  
  Version: 1.0.0
  License: Apache 2.0
 *******************************************************/

#ifndef _Ultrasonic_Sensor_MODBUS_H
#define _Ultrasonic_Sensor_MODBUS_H

#include <Arduino.h>

// read/write registers addresses
#define WRITE_REG     "06"
#define READ_REG      "03"

// reading holding registers addresses
#define ADDRESS_REG       		"0200"
#define PROCESSED_DISTANCE_REG	"0100"
#define REALTIME_DISTANCE_REG 	"0101"


// number of registers to read
#define ONE16BITS_REG     "0001"

// general Ultrasonic_Sensor ID
#define BROADCAST_ID      	0xFF
#define DEFAULT_ID     	 	0x01
#define TIMEOUT_Ultrasonic_Sensor_REALTIME	  100
#define TIMEOUT_Ultrasonic_Sensor_PROCESSED	  500

class Ultrasonic_Sensor_MODBUS
{
    public:
        Ultrasonic_Sensor_MODBUS(Stream *port);
		int ReadAddress();
		bool SetAddress(byte newId);
		bool CheckAddress(byte id);
		bool ChangeAddress(byte oldID , byte newId);
		float ReadUltrasonic_Sensor(byte id, String reg);
		bool RS485_TRANSEIVER(String msgSent , byte numOfSentByte, byte* msgReceived, byte numOfReceivedByte, int receiveTimeout);
		
    private:
		Stream *_port;
		int available();
		int peek();
		int read();
		size_t write(uint8_t c);
		byte StrtoByte (String str_value);
		String ModRTU_CRC(String raw_msg_data);
};

#endif
