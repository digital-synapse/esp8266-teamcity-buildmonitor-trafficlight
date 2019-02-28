// WebServerHelper.h

#ifndef _WEBSERVERHELPER_h
#define _WEBSERVERHELPER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#endif
#include <ESP8266WebServer.h>
extern ESP8266WebServer *server;