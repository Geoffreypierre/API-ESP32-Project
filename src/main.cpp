#include <WiFi.h>
#include <WebServer.h>
#include "config.h"
#include "pins.h"
#include "sensors.h"
#include "led_controller.h"
#include "api_handlers.h"

const char *WIFI_SSID = "Livebox-74F0";
const char *WIFI_PASSWORD = "Aaqy2gtC9AivP2uGgC";

WebServer server(SERVER_PORT);

void setupWiFi()
{
  Serial.println("Connecting to WiFi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup()
{
  Serial.begin(115200);

  LEDController::initialize();
  SensorManager::initialize();

  setupWiFi();

  APIHandlers::setupRoutes(server);

  server.begin();
  Serial.println("HTTP server started");
}

void loop()
{
  server.handleClient();
  LEDController::update();

  delay(SENSOR_READ_DELAY);
}