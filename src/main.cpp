#include <cmath>

#include <Arduino.h>

#include "led.hpp"
#include "vfd.hpp"
#include "switch.hpp"

namespace {
    constexpr const uint32_t uart_baud_bps = 115200;

    constexpr const uint8_t gpio_uart2_rx = 16;
    constexpr const uint8_t gpio_uart2_tx = 17;

    constexpr const uint8_t gpio_sw_a = GPIO_NUM_32;
    constexpr const uint8_t gpio_sw_b = GPIO_NUM_33;
    constexpr const uint8_t gpio_sw_c = GPIO_NUM_34;
    constexpr const uint8_t gpio_sw_d = GPIO_NUM_35;

    LED led;

    VFD vfd;

    Switch sw_a(gpio_sw_a);
    Switch sw_b(gpio_sw_b);
    // GPIO34, 35: 内蔵プルアップ使用不可 (外部プルアップ付けた)
    Switch sw_c(gpio_sw_c, INPUT);
    Switch sw_d(gpio_sw_d, INPUT);

    uint8_t digit = 9;

    uint8_t value_0 = 0;
    int32_t value_1 = 0;
}

void digit_left()
{
    if (digit <= 1)
    {
        return;
    }

    if (digit == 5)
    {
        digit = 2;
        return;
    }

    digit--;
}

void digit_right()
{
    if (digit >= 9)
    {
        return;
    }

    if (digit == 2)
    {
        digit = 5;
        return;
    }

    digit++;
}

void value_up()
{
    switch (digit)
    {
        case 1:
            value_0 += 10;
            break;
        case 2:
            value_0++;
            break;
        case 5:
            value_1 += 10000;
            break;
        case 6:
            value_1 += 1000;
            break;
        case 7:
            value_1 += 100;
            break;
        case 8:
            value_1 += 10;
            break;
        case 9:
            value_1++;
            break;
    }

    if (value_0 >= 99)
    {
        value_0 = 99;
    }

    if (value_1 >= 99999)
    {
        value_1 = 99999;
    }
}

void value_down()
{
    switch (digit)
    {
        case 1:
        
            value_0 = value_0 <= 10 ? 0 : value_0 - 10;
            break;
        case 2:
            value_0 = value_0 <= 0 ? 0 : value_0 - 1;
            break;
        case 5:
            value_1 -= 10000;
            break;
        case 6:
            value_1 -= 1000;
            break;
        case 7:
            value_1 -= 100;
            break;
        case 8:
            value_1 -= 10;
            break;
        case 9:
            value_1--;
            break;
    }

    if (value_1 <= -99999)
    {
        value_1 = -99999;
    }
}

void show_digit(const VFDGrid grid)
{
    if (grid == VFDGrid::Grid4)
    {
        if (value_1 < 0)
        {
            vfd.show_number(grid, 0x10);
        }
        return;
    }

    uint8_t value = 0;
    bool is_visible = millis() % 500 < 250;

    switch (grid)
    {
        // value_0
        case VFDGrid::Grid1:
            value = value_0 / 10;
            is_visible |= digit != 1;
            break;
        case VFDGrid::Grid2:
            value = value_0 % 10;
            is_visible |= digit != 2;
            break;
        // value_1
        case VFDGrid:: Grid5:
            value = std::abs(value_1 / 10000);
            is_visible |= digit != 5;
            break;
        case VFDGrid:: Grid6:
            value = std::abs((value_1 % 10000) / 1000);
            is_visible |= digit != 6;
            break;
        case VFDGrid:: Grid7:
            value = std::abs((value_1 % 1000) / 100);
            is_visible |= digit != 7;
            break;
        case VFDGrid:: Grid8:
            value = std::abs((value_1 % 100) / 10);
            is_visible |= digit != 8;
            break;
        case VFDGrid:: Grid9:
            value = std::abs(value_1 % 10);
            is_visible |= digit != 9;
            break;
    }

    if (is_visible)
    {
        vfd.show_number(grid, value);
    }
}

void setup()
{
    Serial.begin(uart_baud_bps);
    Serial2.begin(uart_baud_bps, SERIAL_8N1, gpio_uart2_rx, gpio_uart2_tx);

    led.begin();

    sw_a.begin();
    sw_a.on_pressed = digit_right;
    sw_b.begin();
    sw_b.on_pressed = digit_left;
    sw_c.begin();
    sw_c.on_pressed = value_down;
    sw_d.begin();
    sw_d.on_pressed = value_up;

    vfd.begin();
}

void loop()
{
    if (millis() % 500 < 250)
    {
        led.on(LEDColor::White);
    }
    else
    {
        led.off();
    }

    sw_a.update();
    sw_b.update();
    sw_c.update();
    sw_d.update();
    
    show_digit(VFDGrid::Grid1);
    show_digit(VFDGrid::Grid2);

    show_digit(VFDGrid::Grid4);
    show_digit(VFDGrid::Grid5);
    show_digit(VFDGrid::Grid6);
    show_digit(VFDGrid::Grid7);
    show_digit(VFDGrid::Grid8);
    show_digit(VFDGrid::Grid9);
}