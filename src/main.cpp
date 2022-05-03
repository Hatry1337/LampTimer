#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include "ESPAsyncWebServer.h"
#include "WebSocket.h"
#include "LittleFS.h"
#include "ArduinoJson.h"
#include "Lamp.h"

#define RELAY_PIN 4                //NodeMCU D2 pin
#define RELAY_OPEN_LEVEL LOW

#define WIFIAP_NAME "ESPLampTimer"
#define WIFIAP_PASS "ONtPpyTppq71"

AsyncWebServer server(80);
AsyncWebSocket socket("/socket");
Lamp lamp(RELAY_PIN, RELAY_OPEN_LEVEL);

void onWebSocketError(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){
	Serial.printf("ws[%s][%u] error(%u): %s\n", server->url(), client->id(), *((uint16_t*)arg), (char*)data);
}

void onWebSocketMessage(AsyncWebSocket * server, AsyncWebSocketClient * client, std::string message) {
    StaticJsonDocument<256> doc;
    DeserializationError error = deserializeJson(doc, message);

	if (error) {
		Serial.print(F("deserializeJson() failed: "));
		Serial.println(error.f_str());
		return;
	}

    JsonObject req_data = doc.as<JsonObject>();

    if (!req_data.containsKey("op")) {
        client->text("{\"status\": \"error\", \"message\": \"no 'op' param specified\"}");
        return;
    }

    if (req_data["op"] == "enable") {
		lamp.turnOn();
        client->text("{\"status\": \"success\"}");
        return;
    }

	if (req_data["op"] == "disable") {
        lamp.turnOff();
        client->text("{\"status\": \"success\"}");
        return;
    }

	if (req_data["op"] == "set-timer") {
		if (!req_data.containsKey("time")) {
			client->text("{\"status\": \"error\", \"message\": \"no 'time' param specified\"}");
			return;
		}
		uint64_t time = req_data["time"];
		lamp.setTimer(time);
        client->text("{\"status\": \"success\"}");
        return;
    }

	if (req_data["op"] == "clear-timer") {
		lamp.clearTimer();
        client->text("{\"status\": \"success\"}");
        return;
    }

	if (req_data["op"] == "time-remaining") {
		uint64_t time = lamp.getTimeRemaining();
        client->printf("{\"status\": \"success\", \"time\": %llu}", time);
        return;
    }

    client->text("{\"status\": \"error\", \"message\": \"unknown op.\"}");
    return;
}

WebSocket socketHandler(onWebSocketMessage, onWebSocketError);

void setup() {
	Serial.begin(9600);

	if(!LittleFS.begin()){
		Serial.println("An Error has occurred while mounting LittleFS");
		return;
  	}

	Serial.print("Starting Wifi AP...");
  	WiFi.softAP(WIFIAP_NAME, WIFIAP_PASS);

	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());

	socket.onEvent(socketHandler.eventHandler);
	server.addHandler(&socket);

	server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    	request->send(LittleFS, "/index.html", "text/html", false);
  	});

	server.on("/main.js", HTTP_GET, [](AsyncWebServerRequest *request){
    	request->send(LittleFS, "/main.js", "text/javascript", false);
  	});

	server.begin();
  	Serial.println("HTTP server started");
}

void loop() {
	lamp.think();
}