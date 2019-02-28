// EEPROMStorage.h
#include <EEPROM.h> //The EEPROM libray 

#ifndef _EEPROMSTORAGE_h
#define _EEPROMSTORAGE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#endif

bool writeString(String data, int position);
String readString(int position);
bool writeInt(int value, int position);
int readInt(int position);
bool writeByte(uint8 value, int position);
uint8 readByte(int position);
