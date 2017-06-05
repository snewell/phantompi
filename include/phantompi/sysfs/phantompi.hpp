#ifndef PHANTOMPI_SYSFS_PHANTOMPI_HPP
#define PHANTOMPI_SYSFS_PHANTOMPI_HPP 1

#include <array>
#include <cstdio>

#include <phantompi/file.hpp>
#include <phantompi/handle.hpp>
#include <phantompi/sysfs/gpio.hpp>

namespace phantompi
{
    namespace sysfs
    {
        template <typename T>
        inline auto accessGpio(std::uint8_t id)
        {
            auto writeFile = [id](char const * path) {
                std::array<char, 3>
                    buffer; // two bytes for id, one for null-terminator
                auto size =
                    std::snprintf(buffer.data(), buffer.size(), "%d", id);

                OutputFile file{path};
                file.write(buffer.data(), size);
            };

            auto closeFn = [writeFile]() {
                writeFile("/sys/class/gpio/unexport");
            };

            writeFile("/sys/class/gpio/export");

            try
            {
                T t{id};
                return makeHandle(std::move(t), closeFn);
            }
            catch(...)
            {
                closeFn();
                throw;
            }
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

#endif
