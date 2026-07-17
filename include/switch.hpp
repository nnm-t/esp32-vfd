#pragma once

#include <functional>

#include <Arduino.h>

class Switch
{
    const uint8_t _gpio;
    const uint8_t _on_state;
    const uint32_t _threshold_ms;

    bool _is_pressed = false;
    uint32_t _last_event_ms = 0;

public:
    std::function<void()> on_pressed = nullptr;
    std::function<void()> on_released = nullptr;

    Switch(const uint8_t gpio, const uint8_t on_state = LOW, const uint32_t threshold_ms = 100) : _gpio(gpio), _on_state(on_state), _threshold_ms(threshold_ms)
    {

    }

    void begin(const uint8_t mode = INPUT_PULLUP);

    void update();
};