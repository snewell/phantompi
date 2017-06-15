#ifndef PHANTOMPI_MAPPED_GPIO_HPP
#define PHANTOMPI_MAPPED_GPIO_HPP

#include <phantompi/mapped/memmap.hpp>

namespace phantompi
{
    namespace mapped
    {
        class Gpio
        {
        public:
            std::uint8_t id() const noexcept;

        protected:
            enum class GpioDirection
            {
                IN,
                OUT
            };

            Gpio(std::uint8_t id, GpioDirection direction);

            std::shared_ptr<GpioMap> map;

        private:
            std::uint8_t const _id;
        };

        class OutputGpio final : public Gpio
        {
        public:
            OutputGpio(std::uint8_t id);

            void setState(GpioState state);
        };

        class InputGpio final : public Gpio
        {
        public:
            InputGpio(std::uint8_t id);

            GpioState getState() const;
        };

        inline std::uint8_t Gpio::id() const noexcept
        {
            return _id;
        }

        inline Gpio::Gpio(std::uint8_t id, GpioDirection direction)
          : map{GpioMap::get()},
            _id{id}
        {
            if(direction == GpioDirection::OUT)
                map->setGpioFunction(id, GpioMap::GPIOFunction::OUTPUT);
            else
                map->setGpioFunction(id, GpioMap::GPIOFunction::INPUT);
        }

        inline OutputGpio::OutputGpio(std::uint8_t id)
          : Gpio{id, Gpio::GpioDirection::OUT}
        {
        }

        inline void OutputGpio::setState(GpioState state)
        {
            if(state == GpioState::high)
                map->setGpio(id());
            else
                map->clearGpio(id());
        }

        inline InputGpio::InputGpio(std::uint8_t id)
          : Gpio{id, Gpio::GpioDirection::IN}
        {
        }

        inline GpioState InputGpio::getState() const
        {
            return map->getGpioValue(id());
        }
    }
}

#endif // PHANTOMPI_MAPPED_GPIO_HPP
