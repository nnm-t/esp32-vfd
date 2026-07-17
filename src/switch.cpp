#include "switch.hpp"

void Switch::begin(const uint8_t mode)
{
    pinMode(_gpio, mode);
}

void Switch::update()
{
    if (millis() < _last_event_ms + _threshold_ms)
    {
        return;
    }

    const bool is_press = digitalRead(_gpio) == _on_state;

    if (_is_pressed == is_press)
    {
        return;
    }

    _is_pressed = is_press;
    _last_event_ms = millis();

    if (is_press && on_pressed != nullptr)
    {
        on_pressed();
    }
    else if (!is_press && on_released != nullptr)
    {
        on_released();
    }
}