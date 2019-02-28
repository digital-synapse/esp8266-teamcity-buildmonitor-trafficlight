// WiFiSetup.h

#ifndef _WIFISETUP_h
#define _WIFISETUP_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#endif

#include <EEPROM.h>
#include "EEPROMStorage.h"
#include "DNSServer.h"
#include <ESP8266WiFi.h>
#include "./DNSServer.h"                  // Patched lib
#include <ESP8266WebServer.h>
#include <WiFiClient.h>

String connectWiFi();
bool connectAPModeCaptiveNetworkServer();
void handleAPmodeClient();
void reboot();
ESP8266WebServer* getWebServer();