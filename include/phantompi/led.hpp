#ifndef PHANTOMPI_LED_HPP
#define PHANTOMPI_LED_HPP 1

#include <phantompi/types.hpp>

namespace phantompi
{
    /** @brief a single-color light-emitting diode
     *
     * This is a helper class to deal with LEDs.  Instances should be created
     * using makeLed().
     *
     * @tparam OUTPUT_GPIO
     *      a device that implements the @a OUTPUT_GPIO concept
     *
     * @sa makeLed
     *
     * @ingroup devices
     */
    template <typename OUTPUT_GPIO>
    class Led
    {
    public:
        /** @brief construct an Led
         *
         * @param output A device that supports having its state written.  @a
         * output is used to control power to the Led.
         */
        Led(OUTPUT_GPIO & output);

        /// @brief turn an Led on
        void turnOn();

        /// @brief turn an Led off
        void turnOff();

    private:
        OUTPUT_GPIO * _outputGpio;
    };

    /** @brief make an Led object
     *
     * @param output a device that implements the @a OUTPUT_GPIO concept
     */
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
