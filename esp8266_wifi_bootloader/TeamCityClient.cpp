#include "Globals.h"
#ifdef INCLUDE_TEAMCITY

	#include "TeamCityClient.h"
	#include <ESP8266HTTPClient.h>
	#include "WebServerHelper.h"
	#include "EEPROMStorage.h"
	#include "WiFiSetup.h"
	#include "base64.h"

	#define BUILD_SUCCESS 0x1
	#define BUILD_FAILURE 0x2
	#define BUILD_RUNNING 0x3
	#define BUILD_HTTPERROR 0x0
	#define BUILD_UNKNOWN 0x4

	HTTPClient http;
	String tcServer;
	String tcProject;
	bool tcMonitor;
	String tcAuth;
	unsigned long tcFreq;
	unsigned long lastRequest;
	unsigned long tcTimeout;
	unsigned long ledStartTime;
	bool enableRed;
	bool enableGreen;
	bool enableYellow;
	int lastKnownStatus;

	int getLatestBuildStatus() {
		//http://192.168.40.83/guestAuth/app/rest/builds/?locator=project:OpSuitePos,count:1
		String url = "http://"+tcServer+"/httpAuth/app/rest/builds/?locator=project:"+tcProject+",count:1,running:any";
		http.setTimeout(10000);
		http.begin(url);
		http.addHeader("Authorization", "Basic " + tcAuth);
		int status = http.GET();
		int code = BUILD_HTTPERROR;

		if (status == 200) {
			String result= http.getString();
			if (result == "") code= BUILD_HTTPERROR;
			else if (-1 != result.indexOf("state=\"running\"")) code= BUILD_RUNNING;
			else if (-1 != result.indexOf("SUCCESS")) code= BUILD_SUCCESS;		
			else if (-1 != result.indexOf("FAILURE") || -1 != result.indexOf("ERROR")) code= BUILD_FAILURE;		
			else code= BUILD_UNKNOWN;		
		}
		http.end();
		return code;
	}

	void tcClientMonitor() {
		if (tcMonitor) {
			// check status once per minute
			if (millis() - lastRequest > tcFreq) {
				int status = getLatestBuildStatus();
				lastRequest = millis();
				if (status != lastKnownStatus){

					lastKnownStatus = status;
					pinMode(1, OUTPUT);
					pinMode(3, OUTPUT);
					pinMode(15, OUTPUT);

					if (status == BUILD_SUCCESS && enableGreen) {
						digitalWrite(1, LOW);
						digitalWrite(3, LOW);
						digitalWrite(15, HIGH);
						ledStartTime = millis();
					}
					else if (status == BUILD_FAILURE && enableRed) {
						digitalWrite(1, HIGH);
						digitalWrite(3, LOW);
						digitalWrite(15, LOW);
						ledStartTime = millis();
					}
					else if (status == BUILD_RUNNING && enableYellow) {
						digitalWrite(1, LOW);
						digitalWrite(3, HIGH);
						digitalWrite(15, LOW);
						ledStartTime = millis();
					}
					else {
						digitalWrite(1, LOW);
						digitalWrite(3, LOW);
						digitalWrite(15, LOW);
						ledStartTime = 0;
					}
				}
			}

			if (tcTimeout != 0  // LED timeout enabled?
				&& ledStartTime != 0 // LED on?
				&& millis() - ledStartTime > tcTimeout) {
				digitalWrite(1, LOW);
				digitalWrite(3, LOW);
				digitalWrite(15, LOW);
				ledStartTime = 0;
			}
		}
	}

	void tcController() {
		tcServer=readString(2);
		tcAuth = readString(3);
		tcProject = readString(4);
		tcFreq = ((unsigned long)readInt(7)) * 1000;  // mul setting (seconds) by 1000 to get milliseconds
		tcTimeout = ((unsigned long)readInt(8)) * 60 * 1000; // mul setting (min) to get milliseconds
		//uint8 flags = readByte(9);
		enableGreen = true;// (flags & 0x1) != 0;
		enableYellow = true;// (flags & 0x2) != 0;
		enableRed = true; // (flags & 0x4) != 0;

		if (tcServer != "" && tcAuth != "" && tcProject != "") {
			tcMonitor = true;
			if (BUILD_HTTPERROR == getLatestBuildStatus()) {
				tcMonitor = false;	
				writeString("", 0);  // config problem?
				reboot();			 // reset to AP mode
			}
		}

		server->on("/tc", HTTP_GET, []() {

			if (server->arg("server") == "" && server->arg("project") == "") {
				tcMonitor = false;
				server->send(200);
				return;
			}
			if (server->arg("server") != "") {
				tcServer = server->arg("server");
			}
			if (server->arg("project") != "") {
				tcProject = server->arg("project");
			}
			if (server->arg("user") != "" && server->arg("pass") != "") {
				base64 b64;
				tcAuth = b64.encode(server->arg("user") + ":" + server->arg("pass"), false);
			}
			if (server->arg("freq")) {
				tcFreq = server->arg("freq").toInt() * 1000;
			}
			if (server->arg("timeout")) {
				tcTimeout = server->arg("timeout").toInt() * 60 * 1000;
			}
			if (tcServer != "" && tcProject != "") {
				tcMonitor = true;
				if (BUILD_HTTPERROR == getLatestBuildStatus()) {
					tcMonitor = false;
					server->send(503); // service unavailable
					return;
				}
				server->send(200);
				return;
			}
			server->send(400);
			return;
		});


	}

#endif