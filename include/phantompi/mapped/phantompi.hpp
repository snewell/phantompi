#ifndef PHANTOMPI_MAPPED_PHANTOMPI_HPP
#define PHANTOMPI_MAPPED_PHANTOMPI_HPP

#include <phantompi/handle.hpp>
#include <phantompi/mapped/gpio.hpp>

#include <cstdint>

namespace phantompi
{
    namespace mapped
    {
        template <typename T>
        inline auto accessGpio(std::uint8_t id)
        {
            T t{id};
            return makeHandle(std::move(t), [](){});;
        }

        inline auto accessInputGpio(std::uint8_t id)
        {
            return accessGpio<InputGpio>(id);
        }

        inline auto accessOutputGpio(std::uint8_t id)
        {
            return accessGpio<OutputGpio>(id);
        }
    }
}

#endif //PHANTOMPI_MAPPED_PHANTOMPI_HPP
