// 
// 
// 

#include "SystemController.h"
#include "WebServerHelper.h"
#include <ESP8266HTTPClient.h>
#include "EEPROMStorage.h"
#include "WiFiSetup.h"

void systemController() {
	server->on("/system", HTTP_GET, []() {

		if (server->arg("reboot") != "") {
			reboot();
		}

		if (server->arg("reset") != "") {
			writeString("", 0);
			reboot();
		}
	});
}