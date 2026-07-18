#include <cmath>
#include <cstring>
#include <string>

#include <Arduino.h>
#include <WiFi.h>

#include <ArduinoJson.h>

#include "esp_mac.h"
#include "esp_now.h"

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

    bool is_esp_now_init = false;
    uint32_t esp_now_receive_ms = 0;
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

void on_esp_now_received(const esp_now_recv_info_t* esp_now_info, const uint8_t* data, int data_len)
{
    esp_now_receive_ms = millis();

    uint8_t src_addr[6] = { 0 };
    std::memcpy(src_addr, esp_now_info->src_addr, sizeof(src_addr));

    Serial.printf("Receive from %02x:%02x:%02x:%02x:%02x:%02x : ", src_addr[0], src_addr[1], src_addr[2], src_addr[3], src_addr[4], src_addr[5]);

    std::string str((char*)data, data_len);
    Serial.println(str.c_str());

    // JSON Parse
    JsonDocument json_document;
    const DeserializationError err = deserializeJson(json_document, str);

    if (err != DeserializationError::Ok)
    {
        Serial.println("JSON deserialization error.");
        return;
    }

    if (json_document["number"].is<uint8_t>())
    {
        value_0 = json_document["number"].as<uint8_t>();
    }

    if (json_document["value"].is<int32_t>())
    {
        value_1 = json_document["value"].as<int32_t>();
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

    // MAC Address
    uint8_t mac_base[6] = { 0 };

    Serial.print("ESP32 MAC: ");
    if (esp_efuse_mac_get_default(mac_base) != ESP_OK)
    {
        Serial.println("Unknown");
        return;
    }

    Serial.printf("%02x:%02x:%02x:%02x:%02x:%02x\n", mac_base[0], mac_base[1], mac_base[2], mac_base[3], mac_base[4], mac_base[5]);
    
    // ESP-NOW
    WiFi.mode(WIFI_STA);

    if (esp_now_init() != ESP_OK)
    {
        Serial.println("ESP-NOW init is failed.");
        return;
    }

    if (esp_now_register_recv_cb(on_esp_now_received) != ESP_OK)
    {
        Serial.println("ESP-NOW register callback is failed.");
    }

    is_esp_now_init = true;
}

void loop()
{
    // ループ中
    led.on(LEDColor::Yellow);

    if (esp_now_receive_ms + 100 > millis())
    {
        // ESP-NOW受信
        led.on(LEDColor::Blue);
    }
    else if (is_esp_now_init && millis() % 500 < 250)
    {
        // ESP-NOW 初期化済
        led.on(LEDColor::White);
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