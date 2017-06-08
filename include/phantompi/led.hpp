#ifndef PHANTOMPI_LED_HPP
#define PHANTOMPI_LED_HPP 1

#include <phantompi/types.hpp>

namespace phantompi
{
    template <typename OUTPUT_GPIO>
    class Led
    {
    public:
        Led(OUTPUT_GPIO & output);

        void turnOn();

        void turnOff();

    private:
        OUTPUT_GPIO * _outputGpio;
    };

    template <typename OUTPUT_GPIO>
    inline auto makeLed(OUTPUT_GPIO & output)
    {
        return Led<OUTPUT_GPIO>(output);
    }

    template <typename OUTPUT_GPIO>
    inline Led<OUTPUT_GPIO>::Led(OUTPUT_GPIO & output)
      : _outputGpio{&output}
    {
    }

    template <typename OUTPUT_GPIO>
    inline void Led<OUTPUT_GPIO>::turnOn()
    {
        (*_outputGpio)->setState(phantompi::GpioState::high);
    }

    template <typename OUTPUT_GPIO>
    inline void Led<OUTPUT_GPIO>::turnOff()
    {
        (*_outputGpio)->setState(phantompi::GpioState::low);
    }
}

#endif
