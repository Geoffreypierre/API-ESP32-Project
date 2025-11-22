#ifndef LED_CONTROLLER_H
#define LED_CONTROLLER_H

#include <Arduino.h>

class LEDController
{
public:
    static void initialize();

    static void setColor(uint8_t red, uint8_t green, uint8_t blue);
    static void setRed();
    static void setGreen();
    static void setBlue();
    static void off();

    static void linkBlueToLight(bool enable);
    static void linkRedToTemp(bool enable);

    static bool isLightLinked();
    static bool isTempLinked();

    static void update();

private:
    static bool lightLinked;
    static bool tempLinked;

    static void updateLightLink();
    static void updateTempLink();
};

#endif