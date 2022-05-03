#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include "ESPAsyncWebServer.h"
#include "LittleFS.h"
#include "ArduinoJson.h"
#include "Lamp.h"

#define RELAY_PIN 12                //NodeMCU D6 pin
#define RELAY_OPEN_LEVEL LOW

#define WIFIAP_NAME "ESPLampTimer"
#define WIFIAP_PASS "ONtPpyTppq71"

AsyncWebServer server(80);
Lamp lamp(RELAY_PIN, RELAY_OPEN_LEVEL);

void setup() {
	Serial.begin(9600);

	lamp.begin();

	if(!LittleFS.begin()){
		Serial.println("An Error has occurred while mounting LittleFS");
		return;
  	}

	
	Serial.print("Starting Wifi AP...");
  	WiFi.softAP(WIFIAP_NAME, WIFIAP_PASS);
	
	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());

	server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    	request->send(LittleFS, "/index.html", "text/html", false);
  	});

	server.on("/main.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    	request->send(LittleFS, "/main.js", "text/javascript", false);
  	});

	server.on("/api/state", HTTP_GET, [](AsyncWebServerRequest *request) {
		char resp[256];
		sprintf(resp, "{ \"status\": \"ok\", \"state\": %d }", lamp.getState());
    	request->send(200, "application/json", resp);
  	});
	server.on("/api/state/on", HTTP_POST, [](AsyncWebServerRequest *request) {
		lamp.turnOn();
    	request->send(200, "application/json", "{ \"status\": \"ok\" }");
  	});
	server.on("/api/state/off", HTTP_POST, [](AsyncWebServerRequest *request) {
		lamp.turnOff();
    	request->send(200, "application/json", "{ \"status\": \"ok\" }");
  	});

	server.on("/api/timer", HTTP_GET, [](AsyncWebServerRequest *request) {
		char resp[256];
		sprintf(resp, "{ \"status\": \"ok\", \"timer\": %lld }", lamp.getTimeRemaining());
    	request->send(200, "application/json", resp);
  	});
	server.on("/api/timer/set", HTTP_POST, [](AsyncWebServerRequest *request) {
		String time;
		if (request->hasParam("time", true)) {
            time = request->getParam("time", true)->value();
        } else {
			request->send(400, "application/json", "{ \"status\": \"err\", \"message\": \"no time value provided\" }");
			return;
		}

		lamp.setTimer(std::stoi(time.c_str()));
    	request->send(200, "application/json", "{ \"status\": \"ok\" }");
  	});
	server.on("/api/timer/clear", HTTP_POST, [](AsyncWebServerRequest *request) {
		lamp.clearTimer();
    	request->send(200, "application/json", "{ \"status\": \"ok\" }");
  	});

	server.begin();
  	Serial.println("HTTP server started");
}

void loop() {
	lamp.think();
}