// TrafficController.h

#ifndef _TRAFFICCONTROLLER_h
#define _TRAFFICCONTROLLER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#endif
#include <ESP8266WebServer.h>
#include "WebServerHelper.h"
void trafficController();
void trafficLightDemo();
void trafficUpdate();
void trafficLightFlashIP(String ip);