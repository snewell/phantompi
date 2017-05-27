#ifndef PHANTOMPI_SYSFS_IO_HPP
#define PHANTOMPI_SYSFS_IO_HPP 1

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

#include <cstdint>

namespace phantompi
{
    namespace sysfs
    {
        class File
        {
        public:
            File(char const * path);

            ~File() noexcept;

            template <typename BYTE>
            std::size_t read(BYTE       * data,
                            std::size_t   length);

            template <typename BYTE>
            std::size_t write(BYTE        const * data,
                            std::size_t         length);

        private:
            int _fd;
        };

        inline File::File(char const * path)
        {
            _fd = ::open(path, O_RDWR);
        }

        inline File::~File() noexcept
        {
            ::close(_fd);
        }

        template <typename BYTE>
        inline std::size_t File::read(BYTE        * data,
                                    std::size_t   length)
        {
            static_assert(sizeof(BYTE) == 1, "BYTE type is not a byte");

            auto ret = ::read(_fd, data, length);
            return ret;
        }

        template <typename BYTE>
        inline std::size_t File::write(BYTE        const * data,
                                    std::size_t         length)
        {
            static_assert(sizeof(BYTE) == 1, "BYTE type is not a byte");

            std::size_t wrote = 0;
            while(length > 0)
            {
                auto ret = ::write(_fd, data + wrote, length);
                wrote += ret;
                length -= ret;
            }
            return wrote;
        }
    }
}

#endif
