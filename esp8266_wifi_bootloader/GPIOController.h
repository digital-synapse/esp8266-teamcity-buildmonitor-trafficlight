// GPIOController.h

#ifndef _GPIOCONTROLLER_h
#define _GPIOCONTROLLER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#endif
#include <ESP8266WebServer.h>
#include "WebServerHelper.h"

#define READ 0x2

void gpioController();
bool getGpioArg(String name, uint8_t &value);
bool getArg(String name, String &value);
void gpioUpdate();
void setAllPinsLow();