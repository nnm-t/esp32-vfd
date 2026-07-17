#pragma once

#include <Arduino.h>

enum class LEDColor
{
    Red = 0x00,
    Yellow = 0x01,
    Green = 0x02,
    Blue = 0x03,
    White = 0x04
};

class LED
{
    static constexpr const uint8_t gpio_led_a = GPIO_NUM_4;
    static constexpr const uint8_t gpio_led_b = GPIO_NUM_5;
    static constexpr const uint8_t gpio_led_c = GPIO_NUM_14;

public:
    LED()
    {

    }

    void begin();

    void on(const LEDColor color);

    void off();
};