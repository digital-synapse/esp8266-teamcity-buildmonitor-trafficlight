#include "Globals.h"
#ifdef INCLUDE_TRAFFIC

	#include "TrafficController.h"
	unsigned long trafficTimeout;
	unsigned long flashingTimeout;
	unsigned long flashingPeriod = 1000;
	uint8 flashingState;
	bool redFlashing;
	bool yellowFlashing;
	bool greenFlashing;

	void trafficUpdate() {
		if (trafficTimeout != 0 && millis() > trafficTimeout) {
			trafficTimeout = 0;
			pinMode(1, OUTPUT);
			pinMode(3, OUTPUT);
			pinMode(15, OUTPUT);
			digitalWrite(1, LOW);
			digitalWrite(3, LOW);
			digitalWrite(15, LOW);
			redFlashing = false;
			yellowFlashing = false;
			greenFlashing = false;
		}

		if (redFlashing || yellowFlashing || greenFlashing) {
			if (flashingTimeout == 0 || millis() > flashingTimeout)
			{
				flashingTimeout = millis() + 1000;
				if (flashingState == LOW) flashingState = HIGH;
				else flashingState = LOW;
				if (redFlashing) digitalWrite(1, flashingState);
				if (greenFlashing) digitalWrite(15, flashingState);
				if (yellowFlashing) digitalWrite(3, flashingState);
			}		
		}
	}

	void trafficController() {

		// traffic light demo
		server->on("/traffic", HTTP_GET, []() {
			if (server->arg("demo") != "") {
				trafficLightDemo();

				server->send(200);
				return;
			}
			else {
				String time = server->arg("time");
				if (time != "") {
					trafficTimeout = millis() + time.toInt() * 1000;
				}
				String flash = server->arg("flash");
				int flashPeriod = flash.toInt();
				if (flashPeriod != 0) flashingPeriod = flashingPeriod;
				else flashingPeriod = 1000;

				String signal = server->arg("signal");			
				if (signal != "") {
					pinMode(1, OUTPUT);
					pinMode(3, OUTPUT);
					pinMode(15, OUTPUT);

					if (signal == "red" || signal == "stop" || signal == "r") {
						digitalWrite(1, HIGH);
						digitalWrite(3, LOW);
						digitalWrite(15, LOW);
						if (flash != "") {
							redFlashing = true;
							yellowFlashing = false;
							greenFlashing = false;
						}
					}
					else if (signal == "yellow" || signal == "yield" || signal == "y") {
						digitalWrite(1, LOW);
						digitalWrite(3, HIGH);
						digitalWrite(15, LOW);
						if (flash != "") {
							yellowFlashing = true;
							greenFlashing = false;
							redFlashing = false;
						}
					}
					else if (signal == "green" || signal == "go" || signal == "g") {
						digitalWrite(1, LOW);
						digitalWrite(3, LOW);
						digitalWrite(15, HIGH);
						if (flash != "") {
							greenFlashing = true;
							yellowFlashing = false;
							redFlashing = false;
						}
					}
					else if (signal == "none" || signal == "off" || signal == "n") {
						digitalWrite(1, LOW);
						digitalWrite(3, LOW);
						digitalWrite(15, LOW);
						greenFlashing = false;
						yellowFlashing = false;
						redFlashing = false;
					}

					server->send(200);
					return;
				}
				else {
					if (flash != "") {
						redFlashing = true;
						greenFlashing = true;
						yellowFlashing = true;
						server->send(200);
						return;
					}
				}
			}

			server->send(400);
			return;
		});
	}

	void trafficLightDemo() {
		pinMode(1, OUTPUT);
		pinMode(3, OUTPUT);
		pinMode(15, OUTPUT);

		for (int i = 0; i < 10; i++) {
			digitalWrite(1, HIGH);
			digitalWrite(3, LOW);
			digitalWrite(15, LOW);
			delay(100);
			digitalWrite(1, LOW);
			digitalWrite(3, HIGH);
			digitalWrite(15, LOW);
			delay(100);
			digitalWrite(1, LOW);
			digitalWrite(3, LOW);
			digitalWrite(15, HIGH);
			delay(100);
		}
		delay(250);
		digitalWrite(1, LOW);
		digitalWrite(3, LOW);
		digitalWrite(15, LOW);
	}

	void trafficLightFlashIP(String ip) {
		pinMode(1, OUTPUT);
		pinMode(3, OUTPUT);
		pinMode(15, OUTPUT);
		int light = 0;

		int ixStart= ip.lastIndexOf(".");
		if (ixStart == -1) return;

		for (int ix = ixStart; ix < ip.length(); ix++) 
		{	
			String c= String(ip.charAt(ix));
			if (c == ".") delay(1000);
			else {
				int digit = c.toInt();
				for (int r = 0; r < digit; r++) {
					if (light == 0) {
						digitalWrite(1, HIGH);
						digitalWrite(3, LOW);
						digitalWrite(15, LOW);
					}
					else if (light == 1) {
						digitalWrite(1, LOW);
						digitalWrite(3, HIGH);
						digitalWrite(15, LOW);
					}
					else if (light == 2) {
						digitalWrite(1, LOW);
						digitalWrite(3, LOW);
						digitalWrite(15, HIGH);
					}
					delay(300);
					digitalWrite(1, LOW);
					digitalWrite(3, LOW);
					digitalWrite(15, LOW);
					delay(200);
				}
				light++;
				if (light == 3) light = 0;
				delay(500);
			}
		}

		delay(250);
		digitalWrite(1, LOW);
		digitalWrite(3, LOW);
		digitalWrite(15, LOW);
	}

#endif