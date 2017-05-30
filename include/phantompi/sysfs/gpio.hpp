#ifndef PHANTOMPI_SYSFS_GPIO_HPP
#define PHANTOMPI_SYSFS_GPIO_HPP 1

#include <array>
#include <cstdint>
#include <cstdio>

#include <phantompi/file.hpp>

namespace phantompi
{
    namespace sysfs
    {
        class Gpio
        {
        public:
            std::uint8_t id() const noexcept;

        protected:
            Gpio(std::uint8_t         id,
                char         const * direction,
                std::size_t          directionSize);

            char const * valuePath() const noexcept;

        private:
            std::array<char, 32> _valuePath;
            std::uint8_t const _id;
        };

        class OutputGpio final : public Gpio
        {
        public:
            OutputGpio(std::uint8_t id);

            template <typename BYTE>
            std::size_t write(BYTE        const * data,
                            std::size_t         length);
        };

        class InputGpio final : public Gpio
        {
        public:
            InputGpio(std::uint8_t id);

            template <typename BYTE>
            std::size_t read(BYTE        * data,
                            std::size_t   length);

        };

        inline std::uint8_t Gpio::id() const noexcept
        {
            return _id;
        }

        inline Gpio::Gpio(std::uint8_t         id,
                        char         const * direction,
                        std::size_t          directionSize)
        : _id{id}
        {
            auto ret = std::snprintf(_valuePath.data(), _valuePath.size(),
                                    "/sys/class/gpio/gpio%d/value", id);

            std::array<char, 40> directionPath;
            ret = std::snprintf(directionPath.data(), directionPath.size(),
                                "/sys/class/gpio/gpio%d/direction", id);

            OutputFile directionFile{directionPath.data()};
            ret = directionFile.write(direction, directionSize);
        }

        inline char const * Gpio::valuePath() const noexcept
        {
            return _valuePath.data();
        }

        inline OutputGpio::OutputGpio(std::uint8_t id)
        : Gpio{id, "out", 3} { }

        template <typename BYTE>
        inline std::size_t OutputGpio::write(BYTE        const * data,
                                            std::size_t         length)
        {
            OutputFile value{valuePath()};
            return value.write(data, length);
        }

        inline InputGpio::InputGpio(std::uint8_t id)
        : Gpio{id, "in", 2} { }

        template <typename BYTE>
        inline std::size_t InputGpio::read(BYTE        * data,
                                        std::size_t   length)
        {
            InputFile value{valuePath()};
            return value.read(data, length);
        }
    }
}

#endif
