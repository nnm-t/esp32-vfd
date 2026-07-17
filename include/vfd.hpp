#pragma once

#include <Arduino.h>

enum class VFDGrid
{
    Grid2 = 0x00,
    Grid3 = 0x01,
    Grid4 = 0x02,
    Grid5 = 0x03,
    Grid6 = 0x04,
    Grid7 = 0x05,
    Grid8 = 0x06,
    Grid9 = 0x07,
    Grid1 = 0x10,
};

enum class VFDSegment
{
    Dp = 0x00,
    A = 0x01,
    B = 0x02,
    C = 0x03,
    D = 0x04,
    E = 0x05,
    F = 0x06,
    G = 0x07,
    Frac_1_2 = 0x10
};

class VFD
{
    static constexpr const uint8_t gpio_vfd_com_a = GPIO_NUM_25;
    static constexpr const uint8_t gpio_vfd_com_b = GPIO_NUM_26;
    static constexpr const uint8_t gpio_vfd_com_c = GPIO_NUM_27;
    static constexpr const uint8_t gpio_vfd_com_en1 = GPIO_NUM_13;
    static constexpr const uint8_t gpio_vfd_com_en2 = GPIO_NUM_19;

    static constexpr const uint8_t gpio_vfd_seg_a = GPIO_NUM_21;
    static constexpr const uint8_t gpio_vfd_seg_b = GPIO_NUM_22;
    static constexpr const uint8_t gpio_vfd_seg_c = GPIO_NUM_23;
    static constexpr const uint8_t gpio_vfd_seg_en = GPIO_NUM_18;

    void inline enable_grid_2_to_9();
    void inline enable_grid_1_and_segment_1_2();

    void inline enable_segment();

    VFDGrid _last_grid = VFDGrid::Grid2;

public:
    VFD()
    {

    }

    void begin();

    void select_grid(const VFDGrid grid);

    void apply_segment(const VFDSegment segment);

    void show_number(const VFDGrid grid, const uint8_t number, const bool show_dp = false);

    void disable_all();
};