// WebController.h

#ifndef _WEBCONTROLLER_h
#define _WEBCONTROLLER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#endif
#include <ESP8266WebServer.h>
#include "WebServerHelper.h"
void webController();
