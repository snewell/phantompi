#ifndef PHANTOMPI_TYPES_HPP
#define PHANTOMPI_TYPES_HPP 1

namespace phantompi
{
    enum class ButtonState
    {
        not_pressed,
        pressed
    };

    enum class GpioState
    {
        low = 0,
        high = 1
    };
}

#endif
