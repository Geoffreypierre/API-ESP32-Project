#include "api_handlers.h"
#include "sensors.h"
#include "led_controller.h"
#include "config.h"
#include "pins.h"
#include <ArduinoJson.h>

extern WebServer server;

void APIHandlers::setupRoutes(WebServer &srv)
{
    srv.on("/api/sensors", HTTP_GET, handleGetSensors);
    srv.on("/api/photoresistance", HTTP_GET, handleGetLight);
    srv.on("/api/thermoresistance", HTTP_GET, handleGetTemperature);
    srv.on("/api/ledrgb/r", HTTP_PATCH, handleLedRed);
    srv.on("/api/ledrgb/g", HTTP_PATCH, handleLedGreen);
    srv.on("/api/ledrgb/b", HTTP_PATCH, handleLedBlue);
    srv.on("/api/ledrgb/link", HTTP_PATCH, handleLinkBlueToLight);
    srv.on("/api/ledrgb/unlink", HTTP_PATCH, handleUnlinkBlueFromLight);
    srv.on("/api/ledrgb/link_temp", HTTP_PATCH, handleLinkRedToTemp);
    srv.on("/api/ledrgb/unlink_temp", HTTP_PATCH, handleUnlinkRedFromTemp);
}

void APIHandlers::sendError(int code, const char *message)
{
    StaticJsonDocument<200> doc;
    doc["status"] = "error";
    doc["message"] = message;
    doc["code"] = code;

    String response;
    serializeJson(doc, response);
    server.send(code, "application/json", response);
}

void APIHandlers::handleGetSensors()
{
    if (!SensorManager::isInitialized())
    {
        sendError(503, "Sensors not initialized");
        return;
    }

    float tempValue = SensorManager::readTemperature();
    float lightValue = SensorManager::readLight();

    if (tempValue == TEMP_ERROR_VALUE || lightValue == LIGHT_ERROR_VALUE)
    {
        sendError(500, "Failed to read sensor data");
        return;
    }

    StaticJsonDocument<512> doc;
    JsonArray sensors = doc.createNestedArray("sensors");

    JsonObject temp = sensors.createNestedObject();
    temp["id"] = "temperature";
    temp["type"] = "thermistor";
    temp["unit"] = "celsius";
    temp["pin"] = Pins::THERMISTOR;
    temp["value"] = tempValue;

    JsonObject light = sensors.createNestedObject();
    light["id"] = "light";
    light["type"] = "ldr";
    light["unit"] = "percent";
    light["pin"] = Pins::LDR;
    light["value"] = lightValue;

    JsonObject led = sensors.createNestedObject();
    led["id"] = "led_indicator";
    led["type"] = "digital";
    led["pin"] = Pins::LED_INDICATOR;
    led["state"] = digitalRead(Pins::LED_INDICATOR);

    JsonObject rgb = sensors.createNestedObject();
    rgb["id"] = "rgb_led";
    rgb["type"] = "rgb";
    JsonObject pins = rgb.createNestedObject("pins");
    pins["r"] = Pins::RGB_RED;
    pins["g"] = Pins::RGB_GREEN;
    pins["b"] = Pins::RGB_BLUE;

    String response;
    serializeJson(doc, response);
    server.send(200, "application/json", response);
}

void APIHandlers::handleGetLight()
{
    if (!SensorManager::isInitialized())
    {
        sendError(503, "Light sensor not available or not initialized");
        return;
    }

    float lightValue = SensorManager::readLight();

    if (lightValue == LIGHT_ERROR_VALUE)
    {
        sendError(500, "Failed to read light sensor");
        return;
    }

    StaticJsonDocument<256> doc;
    doc["id"] = "light";
    doc["type"] = "ldr";
    doc["unit"] = "percent";
    doc["pin"] = Pins::LDR;
    doc["value"] = lightValue;

    String response;
    serializeJson(doc, response);
    server.send(200, "application/json", response);
}

void APIHandlers::handleGetTemperature()
{
    if (!SensorManager::isInitialized())
    {
        sendError(503, "Temperature sensor not available or not initialized");
        return;
    }

    float tempValue = SensorManager::readTemperature();

    if (tempValue == TEMP_ERROR_VALUE)
    {
        sendError(500, "Failed to read temperature sensor");
        return;
    }

    StaticJsonDocument<256> doc;
    doc["id"] = "temperature";
    doc["type"] = "thermistor";
    doc["unit"] = "celsius";
    doc["pin"] = Pins::THERMISTOR;
    doc["value"] = tempValue;

    String response;
    serializeJson(doc, response);
    server.send(200, "application/json", response);
}

void APIHandlers::handleLedRed()
{
    if (!SensorManager::isInitialized())
    {
        sendError(503, "RGB LED not available or not initialized");
        return;
    }

    LEDController::setRed();
    server.send(200, "application/json", "{\"status\":\"ok\",\"color\":\"red\"}");
}

void APIHandlers::handleLedGreen()
{
    if (!SensorManager::isInitialized())
    {
        sendError(503, "RGB LED not available or not initialized");
        return;
    }

    LEDController::setGreen();
    server.send(200, "application/json", "{\"status\":\"ok\",\"color\":\"green\"}");
}

void APIHandlers::handleLedBlue()
{
    if (!SensorManager::isInitialized())
    {
        sendError(503, "RGB LED not available or not initialized");
        return;
    }

    LEDController::setBlue();
    server.send(200, "application/json", "{\"status\":\"ok\",\"color\":\"blue\"}");
}

void APIHandlers::handleLinkBlueToLight()
{
    if (!SensorManager::isInitialized())
    {
        sendError(503, "Sensor or LED not available");
        return;
    }

    float testValue = SensorManager::readLight();
    if (testValue == LIGHT_ERROR_VALUE)
    {
        sendError(500, "Failed to link: light sensor error");
        return;
    }

    LEDController::linkBlueToLight(true);
    server.send(200, "application/json", "{\"status\":\"ok\",\"link\":\"enabled\",\"color\":\"blue\"}");
}

void APIHandlers::handleUnlinkBlueFromLight()
{
    LEDController::linkBlueToLight(false);
    LEDController::off();
    server.send(200, "application/json", "{\"status\":\"ok\",\"link\":\"disabled\"}");
}

void APIHandlers::handleLinkRedToTemp()
{
    if (!SensorManager::isInitialized())
    {
        sendError(503, "Sensor or LED not available");
        return;
    }

    float testValue = SensorManager::readTemperature();
    if (testValue == TEMP_ERROR_VALUE)
    {
        sendError(500, "Failed to link: temperature sensor error");
        return;
    }

    LEDController::linkRedToTemp(true);
    server.send(200, "application/json", "{\"status\":\"ok\",\"link\":\"enabled\",\"color\":\"red\"}");
}

void APIHandlers::handleUnlinkRedFromTemp()
{
    LEDController::linkRedToTemp(false);
    LEDController::off();
    server.send(200, "application/json", "{\"status\":\"ok\",\"link\":\"disabled\"}");
}