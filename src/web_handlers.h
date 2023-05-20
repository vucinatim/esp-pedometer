#pragma once

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "global_definitions.h"
#include "sensor.h"

// WiFi credentials
extern const char* ssid;  // Your WiFi SSID
extern const char* password;  // Your WiFi password

// Create an instance of the server
extern ESP8266WebServer server;

void setupWiFi();
void setupWebServer();
void handleUserData();
