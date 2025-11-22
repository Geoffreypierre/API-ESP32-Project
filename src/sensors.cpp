#include "sensors.h"
#include "config.h"
#include "pins.h"
#include <Arduino.h>

bool SensorManager::initialized = false;

void SensorManager::initialize()
{
    pinMode(Pins::LDR, INPUT);
    pinMode(Pins::THERMISTOR, INPUT);

    delay(SENSOR_INIT_DELAY);

    float tempTest = readTemperature();
    float lightTest = readLight();

    if (tempTest != TEMP_ERROR_VALUE && lightTest != LIGHT_ERROR_VALUE)
    {
        initialized = true;
        Serial.println("Sensors initialized successfully");
    }
    else
    {
        Serial.println("Warning: Sensor initialization failed");
    }
}

bool SensorManager::isInitialized()
{
    return initialized;
}

bool SensorManager::validateAnalogReading(int rawValue)
{
    return rawValue >= 0 && rawValue <= 4095;
}

float SensorManager::readTemperature()
{
    int rawValue = analogRead(Pins::THERMISTOR);

    if (!validateAnalogReading(rawValue))
    {
        return TEMP_ERROR_VALUE;
    }

    float voltage = rawValue * (3.3 / 4095.0);
    float resistance = (3.3 - voltage) / voltage * THERM_REFERENCE_RES;

    if (resistance <= 0 || resistance > 1000000)
    {
        return TEMP_ERROR_VALUE;
    }

    float tempK = 1.0 / (1.0 / THERM_REFERENCE_TEMP +
                         (1.0 / THERM_BETA) * log(resistance / THERM_REFERENCE_RES));

    return tempK - 273.15;
}

float SensorManager::readLight()
{
    int rawValue = analogRead(Pins::LDR);

    if (!validateAnalogReading(rawValue))
    {
        return LIGHT_ERROR_VALUE;
    }

    return (rawValue / 4095.0) * 100.0;
}