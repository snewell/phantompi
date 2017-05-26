#ifndef PHANTOMPI_PHANTOMPI_HPP
#define PHANTOMPI_PHANTOMPI_HPP 1

#include <array>
#include <cstdio>

#include <phantompi/gpio.hpp>
#include <phantompi/handle.hpp>
#include <phantompi/io.hpp>

namespace phantompi
{
    template <typename T>
    inline auto accessGpio(std::uint8_t id)
    {
        auto writeFile = [id](char const * path) {
            std::array<char, 3> buffer; // two bytes for id, one for null-terminator
            auto size = std::snprintf(buffer.data(), buffer.size(), "%d", id);

            File file{path};
            file.write(buffer.data(), size);
        };

        auto closeFn = [writeFile]() {
            writeFile("/sys/class/gpio/unexport");
        };

        writeFile("/sys/class/gpio/export");

        return makeHandle(T{id}, closeFn);
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

#endif
