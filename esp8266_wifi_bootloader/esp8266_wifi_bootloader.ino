#include "Globals.h"
#include "SystemController.h"
#include "TeamCityClient.h"
#include "WebController.h"
#include "GPIOController.h"
#include "WebServerHelper.h"
#include "TrafficController.h"
#include "WiFiSetup.h"
#include "TrafficController.h"
#include "GPIOController.h"
#include "TeamCityClient.h"
#include "SystemController.h"

ESP8266WebServer* server;
bool APmode;
String localIP;

void setup() {
	Serial.begin(115200);
	setAllPinsLow();

	localIP = connectWiFi();
	if (localIP != "") setupWebServer();			
	else APmode= connectAPModeCaptiveNetworkServer();	
}

void loop() {
	if (APmode) handleAPmodeClient();
	else handleWebModeClient();
}

void setupWebServer() {
	server = getWebServer();

#ifdef INCLUDE_TRAFFIC
	trafficController();
#endif

	gpioController();

#ifdef INCLUDE_TEAMCITY
	tcController();
#endif

	webController();
	systemController();
	Serial.println("starting web service...");
	server->begin();

#ifdef INCLUDE_TRAFFIC
	//trafficLightDemo();
	trafficLightFlashIP(localIP); // flash IP 
#endif
}

void handleWebModeClient() {
	server->handleClient();
	gpioUpdate();

#ifdef INCLUDE_TRAFFIC
	trafficUpdate();
#endif

#ifdef INCLUDE_TEAMCITY
	tcClientMonitor();
#endif

}