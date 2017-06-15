#ifndef PHANTOMPI_MAPPED_MEMMAP_HPP
#define PHANTOMPI_MAPPED_MEMMAP_HPP

#include <phantompi/types.hpp>

#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#include <memory>
#include <stdexcept>

namespace phantompi
{
    namespace mapped
    {
        class GpioMap
        {
        public:
            static auto get()
            {
                static std::shared_ptr<GpioMap> inst_;
                if(!inst_)
                {
                    inst_.reset(new GpioMap());
                }

                return inst_;
            }

            enum class GPIOFunction
            {
                INPUT = 000,
                OUTPUT = 001,
                ALT_0 = 100,
                ALT_1 = 101,
                ALT_2 = 110,
                ALT_3 = 111,
                ALT_4 = 011,
                ALT_5 = 010
            };

            void setGpioFunction(std::uint8_t id, GPIOFunction function)
            {
                auto funcSel = getFunctionSelect(id);

                *funcSel &= ~(7 << (((id) % 10) * 3)); // Clear function bits
                *funcSel |= (static_cast<std::uint8_t>(function)
                             << (((id) % 10) * 3)); // Set Function bits
            }

            void setGpio(std::uint8_t id)
            {
                *(gpio + 7) = 1 << id;
            }

            void clearGpio(std::uint8_t id)
            {
                *(gpio + 10) = 1 << id;
            }

            phantompi::GpioState getGpioValue(std::uint8_t id)
            {
                return ((*(gpio + 13) & (1 << id)) > 0)
                           ? phantompi::GpioState::high
                           : phantompi::GpioState::low;
            }

            ~GpioMap()
            {
                munmap(gpio_map, MapLength);
            }

        private:
            GpioMap()
            {
                auto mem = open("/dev/mem", O_RDWR | O_SYNC);
                if(mem < 0)
                    throw std::runtime_error("Failed to open: /dev/mem");

                gpio_map = mmap(nullptr, MapLength, PROT_READ | PROT_WRITE,
                                MAP_SHARED, mem, Gpio_Base_address);

                close(mem);

                if(gpio_map == MAP_FAILED)
                    throw std::runtime_error("Failed to map memory");

                // access through volatile ptr
                gpio = reinterpret_cast<volatile std::uint32_t *>(gpio_map);
            }

            volatile std::uint32_t * getFunctionSelect(std::uint8_t id)
            {
                return gpio + ((id) / 10);
            }

            // RPI 2 & 3
            const std::uint32_t Gpio_Base_address = (0x3F000000 + 0x200000);

            const std::size_t MapLength =
                (4 * 1024); // only need to 0xb0, but most ref use page size

            void * gpio_map;
            volatile std::uint32_t * gpio;
        };
    }
}

#endif // PHANTOMPI_MAPPED_MEMMAP_HPP
