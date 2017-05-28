#ifndef PHANTOMPI_FILE_HPP
#define PHANTOMPI_FILE_HPP 1

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

#include <cstdint>
#include <type_traits>

namespace phantompi
{
    class File
    {
    public:
        enum class Mode : int
        {
            input         = O_RDONLY,
            output        = O_WRONLY,
            bidirectional = O_RDWR
        };

        File(char const * path,
             Mode         mode);

        ~File() noexcept;

    protected:
        int fd() noexcept;

    private:
        int _fd;
    };

    class InputFile : public File
    {
    public:
        InputFile(char const * path);

        template <typename BYTE>
        std::size_t read(BYTE        * data,
                         std::size_t   length);
    };

    class OutputFile : public File
    {
    public:
        OutputFile(char const * path);

        template <typename BYTE>
        std::size_t write(BYTE        const * data,
                          std::size_t         length);
    };

    inline File::File(char const * path,
                      Mode         mode)
    {
        _fd = ::open(path, static_cast<std::underlying_type_t<Mode>>(mode));
    }

    inline File::~File() noexcept
    {
        ::close(_fd);
    }

    inline int File::fd() noexcept
    {
        return _fd;
    }

    inline InputFile::InputFile(char const * path)
      : File{path, Mode::input} { }

    inline OutputFile::OutputFile(char const * path)
      : File{path, Mode::output} { }

    template <typename BYTE>
    inline std::size_t InputFile::read(BYTE        * data,
                                       std::size_t   length)
    {
        static_assert(sizeof(BYTE) == 1, "BYTE type is not a byte");

        auto ret = ::read(fd(), data, length);
        return ret;
    }

    template <typename BYTE>
    inline std::size_t OutputFile::write(BYTE        const * data,
                                         std::size_t         length)
    {
        static_assert(sizeof(BYTE) == 1, "BYTE type is not a byte");

        std::size_t wrote = 0;
        while(length > 0)
        {
            auto ret = ::write(fd(), data + wrote, length);
            wrote += ret;
            length -= ret;
        }
        return wrote;
    }
}

#endif
