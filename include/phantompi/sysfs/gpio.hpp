#ifndef PHANTOMPI_SYSFS_GPIO_HPP
#define PHANTOMPI_SYSFS_GPIO_HPP 1

#include <array>
#include <cassert>
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
            enum class State
            {
                low  = 0,
                high = 1
            };

            std::uint8_t id() const noexcept;

        protected:
            Gpio(std::uint8_t         id,
                char         const * direction,
                std::size_t          directionSize);

            char const * valuePath() const noexcept;

        private:
            using Path = std::array<char, 32>;
            Path _valuePath;
            std::uint8_t const _id;
        };

        class OutputGpio final : public Gpio
        {
        public:
            OutputGpio(std::uint8_t id);

            template <typename BYTE>
            std::size_t write(BYTE        const * data,
                            std::size_t         length);

            void setState(State state);
        };

        class InputGpio final : public Gpio
        {
        public:
            InputGpio(std::uint8_t id);

            State getState() const;
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
#ifndef NDEBUG
            auto ret =
#endif
                std::snprintf(_valuePath.data(), _valuePath.size(),
                              "/sys/class/gpio/gpio%d/value", id);
            assert(ret > 0);
            assert(static_cast<Path::size_type>(ret) < _valuePath.size());

            using DirectionPath = std::array<char, 40>;
            DirectionPath directionPath;
#ifndef NDEBUG
            ret =
#endif
                std::snprintf(directionPath.data(), directionPath.size(),
                              "/sys/class/gpio/gpio%d/direction", id);
            assert(ret > 0);
            assert(static_cast<DirectionPath::size_type>(ret) < _valuePath.size());

            OutputFile directionFile{directionPath.data()};
            auto wrote = directionFile.write(direction, directionSize);
            if(wrote != directionSize)
            {
                throw std::runtime_error{"Failed to set GPIO direction"};
            }
        }

        inline char const * Gpio::valuePath() const noexcept
        {
            return _valuePath.data();
        }

        inline OutputGpio::OutputGpio(std::uint8_t id)
        : Gpio{id, "out", 3} { }

        inline void OutputGpio::setState(State state)
        {
            static char const * strings[2] = { "0", "1" };

            OutputFile value{valuePath()};
            value.write(strings[static_cast<std::underlying_type_t<State>>(state)], 1);
        }

        inline InputGpio::InputGpio(std::uint8_t id)
        : Gpio{id, "in", 2} { }

        inline auto InputGpio::getState() const -> State
        {
            char buffer;
            InputFile value{valuePath()};
            value.read(&buffer, 1);
            return (buffer == '1') ? State::high : State::low;
        }
    }
}

#endif
