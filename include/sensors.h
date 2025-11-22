#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>

class SensorManager
{
public:
    static void initialize();
    static bool isInitialized();

    static float readTemperature();
    static float readLight();

private:
    static bool initialized;
    static bool validateAnalogReading(int rawValue);
};

#endif