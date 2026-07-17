#include "led.hpp"

void LED::begin()
{
    pinMode(gpio_led_a, OUTPUT);
    pinMode(gpio_led_b, OUTPUT);
    pinMode(gpio_led_c, OUTPUT);

    off();
}

void LED::on(const LEDColor color)
{
    switch (color)
    {
        case LEDColor::Red:
            digitalWrite(gpio_led_a, LOW);
            digitalWrite(gpio_led_b, LOW);
            digitalWrite(gpio_led_c, LOW);
            break;
        case LEDColor::Yellow:
            digitalWrite(gpio_led_a, HIGH);
            digitalWrite(gpio_led_b, LOW);
            digitalWrite(gpio_led_c, LOW);
            break;
        case LEDColor::Green:
            digitalWrite(gpio_led_a, LOW);
            digitalWrite(gpio_led_b, HIGH);
            digitalWrite(gpio_led_c, LOW);
            break;
        case LEDColor::Blue:
            digitalWrite(gpio_led_a, HIGH);
            digitalWrite(gpio_led_b, HIGH);
            digitalWrite(gpio_led_c, LOW);
            break;
        case LEDColor::White:
            digitalWrite(gpio_led_a, LOW);
            digitalWrite(gpio_led_b, LOW);
            digitalWrite(gpio_led_c, HIGH);
            break;
    }
}

void LED::off()
{
    digitalWrite(gpio_led_a, HIGH);
    digitalWrite(gpio_led_b, HIGH);
    digitalWrite(gpio_led_c, HIGH);
}