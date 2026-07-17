#include "vfd.hpp"

void VFD::begin()
{
    pinMode(gpio_vfd_com_en1, OUTPUT);
    pinMode(gpio_vfd_com_en2, OUTPUT);

    pinMode(gpio_vfd_com_a, OUTPUT);
    pinMode(gpio_vfd_com_b, OUTPUT);
    pinMode(gpio_vfd_com_c, OUTPUT);

    pinMode(gpio_vfd_seg_en, OUTPUT);

    pinMode(gpio_vfd_seg_a, OUTPUT);
    pinMode(gpio_vfd_seg_b, OUTPUT);
    pinMode(gpio_vfd_seg_c, OUTPUT);

    disable_all();
}

void VFD::disable_all()
{
    digitalWrite(gpio_vfd_com_en1, LOW);
    digitalWrite(gpio_vfd_com_en2, LOW);
    digitalWrite(gpio_vfd_seg_en, LOW);
    delayMicroseconds(10);
}

void inline VFD::enable_grid_2_to_9()
{
    digitalWrite(gpio_vfd_com_en1, HIGH);
    digitalWrite(gpio_vfd_com_en2, LOW);
}

void inline VFD::enable_grid_1_and_segment_1_2()
{
    digitalWrite(gpio_vfd_com_en1, LOW);
    digitalWrite(gpio_vfd_com_en2, HIGH);
    digitalWrite(gpio_vfd_seg_en, LOW);
}

void inline VFD::enable_segment()
{
    digitalWrite(gpio_vfd_seg_en, HIGH);
}

void VFD::select_grid(const VFDGrid grid)
{
    if (grid != _last_grid)
    {
        _last_grid = grid;
        disable_all();
    }

    switch (grid)
    {
        case VFDGrid::Grid2:
            enable_grid_2_to_9();
            digitalWrite(gpio_vfd_com_a, LOW);
            digitalWrite(gpio_vfd_com_b, LOW);
            digitalWrite(gpio_vfd_com_c, LOW);
            break;
        case VFDGrid::Grid3:
            enable_grid_2_to_9();
            digitalWrite(gpio_vfd_com_a, HIGH);
            digitalWrite(gpio_vfd_com_b, LOW);
            digitalWrite(gpio_vfd_com_c, LOW);
            break;
        case VFDGrid::Grid4:
            enable_grid_2_to_9();
            digitalWrite(gpio_vfd_com_a, LOW);
            digitalWrite(gpio_vfd_com_b, HIGH);
            digitalWrite(gpio_vfd_com_c, LOW);
            break;
        case VFDGrid::Grid5:
            enable_grid_2_to_9();
            digitalWrite(gpio_vfd_com_a, HIGH);
            digitalWrite(gpio_vfd_com_b, HIGH);
            digitalWrite(gpio_vfd_com_c, LOW);
            break;
        case VFDGrid::Grid6:
            enable_grid_2_to_9();
            digitalWrite(gpio_vfd_com_a, LOW);
            digitalWrite(gpio_vfd_com_b, LOW);
            digitalWrite(gpio_vfd_com_c, HIGH);
            break;
        case VFDGrid::Grid7:
            enable_grid_2_to_9();
            digitalWrite(gpio_vfd_com_a, HIGH);
            digitalWrite(gpio_vfd_com_b, LOW);
            digitalWrite(gpio_vfd_com_c, HIGH);
            break;
        case VFDGrid::Grid8:
            enable_grid_2_to_9();
            digitalWrite(gpio_vfd_com_a, LOW);
            digitalWrite(gpio_vfd_com_b, HIGH);
            digitalWrite(gpio_vfd_com_c, HIGH);
            break;
        case VFDGrid::Grid9:
            enable_grid_2_to_9();
            digitalWrite(gpio_vfd_com_a, HIGH);
            digitalWrite(gpio_vfd_com_b, HIGH);
            digitalWrite(gpio_vfd_com_c, HIGH);
            break;
        case VFDGrid::Grid1:
            enable_grid_1_and_segment_1_2();
            digitalWrite(gpio_vfd_com_a, LOW);
            digitalWrite(gpio_vfd_com_b, LOW);
            digitalWrite(gpio_vfd_com_c, LOW);
            break;
    }
}

void VFD::apply_segment(const VFDSegment segment)
{
    switch (segment)
    {
        case VFDSegment::Dp:
            enable_segment();
            digitalWrite(gpio_vfd_seg_a, LOW);
            digitalWrite(gpio_vfd_seg_b, LOW);
            digitalWrite(gpio_vfd_seg_c, LOW);
            break;
        case VFDSegment::A:
            enable_segment();
            digitalWrite(gpio_vfd_seg_a, HIGH);
            digitalWrite(gpio_vfd_seg_b, LOW);
            digitalWrite(gpio_vfd_seg_c, LOW);
            break;
        case VFDSegment::B:
            enable_segment();
            digitalWrite(gpio_vfd_seg_a, LOW);
            digitalWrite(gpio_vfd_seg_b, HIGH);
            digitalWrite(gpio_vfd_seg_c, LOW);
            break;
        case VFDSegment::C:
            enable_segment();
            digitalWrite(gpio_vfd_seg_a, HIGH);
            digitalWrite(gpio_vfd_seg_b, HIGH);
            digitalWrite(gpio_vfd_seg_c, LOW);
            break;
        case VFDSegment::D:
            enable_segment();
            digitalWrite(gpio_vfd_seg_a, LOW);
            digitalWrite(gpio_vfd_seg_b, LOW);
            digitalWrite(gpio_vfd_seg_c, HIGH);
            break;
        case VFDSegment::E:
            enable_segment();
            digitalWrite(gpio_vfd_seg_a, HIGH);
            digitalWrite(gpio_vfd_seg_b, LOW);
            digitalWrite(gpio_vfd_seg_c, HIGH);
            break;
        case VFDSegment::F:
            enable_segment();
            digitalWrite(gpio_vfd_seg_a, LOW);
            digitalWrite(gpio_vfd_seg_b, HIGH);
            digitalWrite(gpio_vfd_seg_c, HIGH);
            break;
        case VFDSegment::G:
            enable_segment();
            digitalWrite(gpio_vfd_seg_a, HIGH);
            digitalWrite(gpio_vfd_seg_b, HIGH);
            digitalWrite(gpio_vfd_seg_c, HIGH);
            break;
        case VFDSegment::Frac_1_2:
            enable_grid_1_and_segment_1_2();
            digitalWrite(gpio_vfd_com_a, HIGH);
            digitalWrite(gpio_vfd_com_b, LOW);
            digitalWrite(gpio_vfd_com_c, LOW);
            break;
    }
}

void VFD::show_number(const VFDGrid grid, const uint8_t number, const bool show_dp)
{
    select_grid(grid);

    switch (number)
    {
        case 0:
            apply_segment(VFDSegment::A);
            delayMicroseconds(117);
            apply_segment(VFDSegment::B);
            delayMicroseconds(117);
            apply_segment(VFDSegment::C);
            delayMicroseconds(117);
            apply_segment(VFDSegment::D);
            delayMicroseconds(117);
            apply_segment(VFDSegment::E);
            delayMicroseconds(117);
            apply_segment(VFDSegment::F);
            delayMicroseconds(117);
            break;
        case 1:
            apply_segment(VFDSegment::B);
            delayMicroseconds(350);
            apply_segment(VFDSegment::C);
            delayMicroseconds(350);
            break;
        case 2:
            apply_segment(VFDSegment::A);
            delayMicroseconds(140);
            apply_segment(VFDSegment::B);
            delayMicroseconds(140);
            apply_segment(VFDSegment::D);
            delayMicroseconds(140);
            apply_segment(VFDSegment::E);
            delayMicroseconds(140);
            apply_segment(VFDSegment::G);
            delayMicroseconds(140);
            break;
        case 3:
            apply_segment(VFDSegment::A);
            delayMicroseconds(140);
            apply_segment(VFDSegment::B);
            delayMicroseconds(140);
            apply_segment(VFDSegment::C);
            delayMicroseconds(140);
            apply_segment(VFDSegment::D);
            delayMicroseconds(140);
            apply_segment(VFDSegment::G);
            delayMicroseconds(140);
            break;
        case 4:
            apply_segment(VFDSegment::B);
            delayMicroseconds(188);
            apply_segment(VFDSegment::C);
            delayMicroseconds(188);
            apply_segment(VFDSegment::F);
            delayMicroseconds(188);
            apply_segment(VFDSegment::G);
            delayMicroseconds(188);
            break;
        case 5:
            apply_segment(VFDSegment::A);
            delayMicroseconds(140);
            apply_segment(VFDSegment::C);
            delayMicroseconds(140);
            apply_segment(VFDSegment::D);
            delayMicroseconds(140);
            apply_segment(VFDSegment::F);
            delayMicroseconds(140);
            apply_segment(VFDSegment::G);
            delayMicroseconds(140);
            break;
        case 6:
            apply_segment(VFDSegment::A);
            delayMicroseconds(117);
            apply_segment(VFDSegment::C);
            delayMicroseconds(117);
            apply_segment(VFDSegment::D);
            delayMicroseconds(117);
            apply_segment(VFDSegment::E);
            delayMicroseconds(117);
            apply_segment(VFDSegment::F);
            delayMicroseconds(117);
            apply_segment(VFDSegment::G);
            delayMicroseconds(117);
            break;
        case 7:
            apply_segment(VFDSegment::A);
            delayMicroseconds(233);
            apply_segment(VFDSegment::B);
            delayMicroseconds(233);
            apply_segment(VFDSegment::C);
            delayMicroseconds(233);
            break;
        case 8:
            apply_segment(VFDSegment::A);
            delayMicroseconds(100);
            apply_segment(VFDSegment::B);
            delayMicroseconds(100);
            apply_segment(VFDSegment::C);
            delayMicroseconds(100);
            apply_segment(VFDSegment::D);
            delayMicroseconds(100);
            apply_segment(VFDSegment::E);
            delayMicroseconds(100);
            apply_segment(VFDSegment::F);
            delayMicroseconds(100);
            apply_segment(VFDSegment::G);
            delayMicroseconds(100);
            break;
        case 9:
            apply_segment(VFDSegment::A);
            delayMicroseconds(117);
            apply_segment(VFDSegment::B);
            delayMicroseconds(117);
            apply_segment(VFDSegment::C);
            delayMicroseconds(117);
            apply_segment(VFDSegment::D);
            delayMicroseconds(117);
            apply_segment(VFDSegment::F);
            delayMicroseconds(117);
            apply_segment(VFDSegment::G);
            delayMicroseconds(117);
            break;
        default:
            if (grid == VFDGrid::Grid9)
            {
                // 1/2
                apply_segment(VFDSegment::Frac_1_2);
                delayMicroseconds(700);
            }
            else
            {
                // Minus
                apply_segment(VFDSegment::G);
                delayMicroseconds(700);
            }
            break;
    }

    if (show_dp)
    {
        apply_segment(VFDSegment::Dp);
        delayMicroseconds(100);
    }
}