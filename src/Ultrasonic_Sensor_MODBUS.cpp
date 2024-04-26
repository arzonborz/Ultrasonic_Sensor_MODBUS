/****************************************************** 
  Arduino library for communicating though RS-485 Modbus with a chinese djlk ultrasonic sensor
  
  Author: Ghaith alshishani arzonborz@gmail.com
  
  Version: 1.0.0
  License: Apache 2.0
 *******************************************************/

#include "Ultrasonic_Sensor_MODBUS.h"

Ultrasonic_Sensor_MODBUS::Ultrasonic_Sensor_MODBUS(Stream *port)
{
	_port=port;
}

// ReadAddress read and retuns the ID address of the Ultrasonic_Sensor sensor on the RS485 MODBUS. There should only be one Ultrasonic_Sensor sensor connected to the bus at a time because this function uses the broadcast ID address
int Ultrasonic_Sensor_MODBUS::ReadAddress()
{
  return this->ReadUltrasonic_Sensor(BROADCAST_ID,ADDRESS_REG);
}

// SetAddress returns a boolean to determine succeful operation. This function Sets the ID address of the Ultrasonic_Sensor sensor on the RS485 MODBUS. There should only be one Ultrasonic_Sensor sensor connected to the bus at a time because this function uses the broadcast ID address
bool Ultrasonic_Sensor_MODBUS::SetAddress(byte newId)
{
  return this->ChangeAddress(BROADCAST_ID,newId);
}

// CheckAddress returns a boolean and checks if the Sensor ID is present on RS485 MODBUS
bool Ultrasonic_Sensor_MODBUS::CheckAddress(byte id)
{
  return this->ReadUltrasonic_Sensor(id,ADDRESS_REG)==id;
}

// ChangeAddress changes the id address of a given Ultrasonic_Sensor id address if the new address is not already taken on the RS485 MODBUS
bool Ultrasonic_Sensor_MODBUS::ChangeAddress(byte oldID , byte newId)
{
  // check the args
  if ( oldID<0 || oldID >255 || newId<0 || newId > 255) return false;
  // check the if the newId is already taken
  if (newId != BROADCAST_ID) if(this->CheckAddress(newId)) return false;

  // prepare the msg
  String sensor_oldId = String(oldID, HEX);
  String sensor_newId = String(newId, HEX);
  sensor_oldId.toUpperCase();
  sensor_newId.toUpperCase();
  //The address should be like XY. Add zeros if we need it
  if (sensor_oldId.length() == 1) sensor_oldId = "0" + sensor_oldId;
  if (sensor_newId.length() == 1) sensor_newId = "0" + sensor_newId;
  sensor_newId = "00"+sensor_newId;
  
  //Build the message
  String msg = sensor_oldId + String(WRITE_REG) + String(ADDRESS_REG) + sensor_newId;
  msg += this->ModRTU_CRC(msg);

  byte buf[8];
  bool transResult = this->RS485_TRANSEIVER( msg , 8, buf, 8, TIMEOUT_Ultrasonic_Sensor_REALTIME);
  if (!transResult) return false;
  
  // Check if the Ultrasonic_Sensor sensor Address is changed correctly
  if (buf[1]==this->StrtoByte(WRITE_REG) && buf[5]==newId) return true;
  else return false;
}

// reads the data registers from Ultrasonic_Sensor sensors with a specified id. returns (-255) when wrong id is passed (-254) when wrong address is passed (-253 & -252) when no correct bytes were recieved
float Ultrasonic_Sensor_MODBUS::ReadUltrasonic_Sensor(byte id, String reg)
{
  if ( id<0 || id >255 )
  {
    return -255;
  }
  if (reg!=ADDRESS_REG && reg!=PROCESSED_DISTANCE_REG && reg!=REALTIME_DISTANCE_REG )
  {
    return -254;
  }
  
  //Fix address to hex string:
  String sensor_ID = String(id, HEX);
  sensor_ID.toUpperCase();
  //The address should be like XY. Add zeros if we need it
  if (sensor_ID.length() == 1) {
    sensor_ID = "0" + sensor_ID;
  }
  //Build the message
  String msg = sensor_ID + String(READ_REG) + String(reg) + String(ONE16BITS_REG);
  msg += this->ModRTU_CRC(msg);

  //Wait for answer and put it in a buffer
  float retVal;
  byte buf[8];
  uint16_t timeout = reg == PROCESSED_DISTANCE_REG ? TIMEOUT_Ultrasonic_Sensor_PROCESSED : TIMEOUT_Ultrasonic_Sensor_REALTIME;
  bool transResult = this->RS485_TRANSEIVER( msg , 8, buf, 7, timeout);
  if (!transResult) return -253;

  // Check if the Ultrasonic_Sensor sensor is read successfully then decode the retVal
  if (buf[1]==StrtoByte(READ_REG)) 
  {
    retVal = buf[3]<<8 | buf[4];
    return retVal;
  }
  else return -252;
}

// RS485_TRANSEIVER send number of bytes (String) over RS485 MODBUS and waits for number of expected bytes (array)
bool Ultrasonic_Sensor_MODBUS::RS485_TRANSEIVER(String msgSent , byte numOfSentByte, byte* msgReceived, byte numOfReceivedByte, int receiveTimeout)
{
  //check if MODBUS msg is not correctly formed
  if (msgSent.length()<numOfSentByte*2) return false;
  //Send the message
  char msgCharArray[numOfSentByte*2+1];
  msgSent.toCharArray(msgCharArray,17);
  for (int i=0;i<numOfSentByte*2;i+=2)
  {
    byte msgByte = this->StrtoByte(String(msgCharArray[i])+ String(msgCharArray[i+1]));
    this->write(msgByte);
  }

  //Wait for answer and put it in a buffer
  String strBuf="";
  int timeout = receiveTimeout*2;
  int i=0;
  while(this->available()<=0 && timeout>0)
  {
    timeout--;
    delay(1);
  }
  timeout = receiveTimeout;
  while(timeout>0)
  {
    timeout--;
    delay(1);
    if (this->available()<=0)continue;
    msgReceived[i]=this->read();
    String tempstr = String(msgReceived[i], HEX);
    tempstr.toUpperCase();
    //The address should be like XY. Add zeros if we need it
    if (tempstr.length() == 1) tempstr = "0" + tempstr;
    strBuf+= tempstr;
    i++;
    if (i==numOfReceivedByte)break;
  }

  // Check if the response has the correct MODBUS CRC16 ending 
  if (this->ModRTU_CRC(strBuf.substring(0, numOfReceivedByte*2-4)) != strBuf.substring(numOfReceivedByte*2-4, numOfReceivedByte*2)) return false;
  else return true;
}

// Compute the MODBUS RTU CRC
String Ultrasonic_Sensor_MODBUS::ModRTU_CRC(String raw_msg_data) {
  //Calc raw_msg_data length
  byte raw_msg_data_byte[raw_msg_data.length() / 2];
  //Convert the raw_msg_data to a byte array raw_msg_data
  for (int i = 0; i < raw_msg_data.length() / 2; i++) {
    raw_msg_data_byte[i] = this->StrtoByte(raw_msg_data.substring(2 * i, 2 * i + 2));
  }

  //Calc the raw_msg_data_byte CRC code
  uint16_t crc = 0xFFFF;
  String crc_string = "";
  for (int pos = 0; pos < raw_msg_data.length() / 2; pos++) {
    crc ^= (uint16_t)raw_msg_data_byte[pos];          // XOR byte into least sig. byte of crc
    for (int i = 8; i != 0; i--) {    // Loop over each bit
      if ((crc & 0x0001) != 0) {      // If the LSB is set
        crc >>= 1;                    // Shift right and XOR 0xA001
        crc ^= 0xA001;
      }
      else                            // Else LSB is not set
        crc >>= 1;                    // Just shift right
    }
  }
  // Note, this number has low and high bytes swapped, so use it accordingly (or swap bytes)

  //Become crc byte to a capital letter String
  crc_string = String(crc, HEX);
  crc_string.toUpperCase();

  //The crc should be like XXYY. Add zeros if need it
  if (crc_string.length() == 1) {
    crc_string = "000" + crc_string;
  } else if (crc_string.length() == 2) {
    crc_string = "00" + crc_string;
  } else if (crc_string.length() == 3) {
    crc_string = "0" + crc_string;
  } else {
    //OK
  }

  //Invert the byte positions
  crc_string = crc_string.substring(2, 4) + crc_string.substring(0, 2);
  return crc_string;
}

//String to byte --> Example: String = "C4" --> byte = {0xC4}
byte Ultrasonic_Sensor_MODBUS::StrtoByte (String str_value) {
  char char_buff[3];
  str_value.toCharArray(char_buff, 3);
  byte byte_value = strtoul(char_buff, NULL, 16);
  return byte_value;
}

int Ultrasonic_Sensor_MODBUS::available()
{
	return _port->available();
}

int Ultrasonic_Sensor_MODBUS::peek()
{
	return _port->peek();
}

int Ultrasonic_Sensor_MODBUS::read()
{
	return _port->read();
}

size_t Ultrasonic_Sensor_MODBUS::write(uint8_t c)
{
	return _port->write(c);
}

