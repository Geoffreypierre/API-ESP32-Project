#include "led_controller.h"
#include "sensors.h"
#include "config.h"
#include "pins.h"

bool LEDController::lightLinked = false;
bool LEDController::tempLinked = false;

void LEDController::initialize()
{
    pinMode(Pins::LED_INDICATOR, OUTPUT);
    pinMode(Pins::RGB_RED, OUTPUT);
    pinMode(Pins::RGB_GREEN, OUTPUT);
    pinMode(Pins::RGB_BLUE, OUTPUT);

    off();
    digitalWrite(Pins::LED_INDICATOR, LOW);
}

void LEDController::setColor(uint8_t red, uint8_t green, uint8_t blue)
{
    lightLinked = false;
    tempLinked = false;

    analogWrite(Pins::RGB_RED, red);
    analogWrite(Pins::RGB_GREEN, green);
    analogWrite(Pins::RGB_BLUE, blue);
}

void LEDController::setRed()
{
    setColor(255, 0, 0);
}

void LEDController::setGreen()
{
    setColor(0, 255, 0);
}

void LEDController::setBlue()
{
    setColor(0, 0, 255);
}

void LEDController::off()
{
    analogWrite(Pins::RGB_RED, 0);
    analogWrite(Pins::RGB_GREEN, 0);
    analogWrite(Pins::RGB_BLUE, 0);
}

void LEDController::linkBlueToLight(bool enable)
{
    lightLinked = enable;
    if (enable)
    {
        tempLinked = false;
        off();
    }
}

void LEDController::linkRedToTemp(bool enable)
{
    tempLinked = enable;
    if (enable)
    {
        lightLinked = false;
        off();
    }
}

bool LEDController::isLightLinked()
{
    return lightLinked;
}

bool LEDController::isTempLinked()
{
    return tempLinked;
}

void LEDController::updateLightLink()
{
    float lightPercent = SensorManager::readLight();

    if (lightPercent != LIGHT_ERROR_VALUE)
    {
        float factor = 1.0 - (lightPercent / 100.0);
        int pwmValue = (int)(255 * pow(factor, 0.3));
        pwmValue = constrain(pwmValue, 0, 255);

        analogWrite(Pins::RGB_BLUE, pwmValue);

        Serial.print("Light: ");
        Serial.print(lightPercent);
        Serial.print("% -> PWM blue: ");
        Serial.println(pwmValue);
    }
    else
    {
        Serial.println("Warning: Invalid light reading");
    }
}

void LEDController::updateTempLink()
{
    float tempC = SensorManager::readTemperature();

    if (tempC != TEMP_ERROR_VALUE)
    {
        float normalizedTemp = constrain(tempC, TEMP_MIN, TEMP_MAX);
        float factor = (normalizedTemp - TEMP_MIN) / (TEMP_MAX - TEMP_MIN);
        int pwmValue = (int)(255 * pow(factor, 1.5) * 3);
        pwmValue = constrain(pwmValue, 0, 255);

        analogWrite(Pins::RGB_RED, pwmValue);

        Serial.print("Temperature: ");
        Serial.print(tempC);
        Serial.print("°C -> PWM red: ");
        Serial.println(pwmValue);
    }
    else
    {
        Serial.println("Warning: Invalid temperature reading");
    }
}

void LEDController::update()
{
    if (lightLinked)
    {
        updateLightLink();
    }

    if (tempLinked)
    {
        updateTempLink();
    }

    float lightPercent = SensorManager::readLight();
    if (lightPercent >= 0)
    {
        Serial.print("Light: ");
        Serial.print(lightPercent);
        Serial.println("%");
    }
}