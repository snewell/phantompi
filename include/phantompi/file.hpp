#ifndef PHANTOMPI_FILE_HPP
#define PHANTOMPI_FILE_HPP 1

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <cerrno>
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <type_traits>

namespace phantompi
{
    class File
    {
    public:
        enum class Mode : int
        {
            input = O_RDONLY,
            output = O_WRONLY,
            bidirectional = O_RDWR,

            create = O_CREAT
        };

        template <typename... MODES>
        File(char const * path, MODES... modes);

        template <typename... MODES>
        File(char const * path, int permissions, MODES... modes);

        File(File const &) = delete;

        File & operator=(File const &) = delete;

        File(File && file) noexcept;

        File & operator=(File && file) noexcept;

        ~File() noexcept;

    protected:
        int fd() noexcept;

    private:
        static constexpr int buildMode_inner(Mode mode) noexcept;

        template <typename... MODES>
        static constexpr int buildMode_inner(Mode mode,
                                             MODES... modes) noexcept;

        template <typename... MODES>
        static constexpr int buildMode(MODES... modes) noexcept;

        int _fd;
    };

    class InputFile : public File
    {
    public:
        InputFile(char const * path);

        template <typename BYTE>
        std::size_t read(BYTE * data, std::size_t length);
    };

    class OutputFile : public File
    {
    public:
        enum class Permissions
        {
            owner_read = S_IRUSR,
            owner_write = S_IWUSR,
            owner_execute = S_IXUSR,
            group_read = S_IRGRP,
            group_write = S_IWGRP,
            group_execute = S_IXGRP,
            other_read = S_IROTH,
            other_write = S_IWOTH,
            other_execute = S_IXOTH
        };

        OutputFile(char const * path);

        template <typename... PERMISSIONS>
        static OutputFile createFile(char const * path, PERMISSIONS... perms);

        template <typename BYTE>
        std::size_t write(BYTE const * data, std::size_t length);

    private:
        static constexpr int buildPerms_inner(Permissions permission) noexcept;

        template <typename... PERMISSIONS>
        static constexpr int
        buildPerms_inner(Permissions permission,
                         PERMISSIONS... permissions) noexcept;

        template <typename... PERMISSIONS>
        static constexpr int
        buildPermissions(PERMISSIONS... permissions) noexcept;

        template <typename... MODES>
        OutputFile(char const * path, int permissions, MODES... modes);
    };

    constexpr int File::buildMode_inner(Mode mode) noexcept
    {
        return static_cast<std::underlying_type_t<Mode>>(mode);
    }

    template <typename... MODES>
    constexpr int File::buildMode_inner(Mode mode, MODES... modes) noexcept
    {
        return buildMode_inner(mode) | buildMode_inner(modes...);
    }

    template <typename... MODES>
    constexpr int File::buildMode(MODES... modes) noexcept
    {
        return buildMode_inner(modes...);
    }

    template <typename... MODES>
    inline File::File(char const * path, MODES... modes)
    {
        _fd = ::open(path, buildMode(modes...));
        if(_fd == -1)
        {
            auto error = errno;
            throw std::runtime_error{::strerror(error)};
        }
    }

    template <typename... MODES>
    inline File::File(char const * path, int permissions, MODES... modes)
    {
        _fd = ::open(path, buildMode(modes...), permissions);
        if(_fd == -1)
        {
            auto error = errno;
            throw std::runtime_error{::strerror(error)};
        }
    }

    inline File::File(File && file) noexcept
      : _fd{file._fd}
    {
        file._fd = -1;
    }

    inline File & File::operator=(File && file) noexcept
    {
        std::swap(_fd, file._fd);
        return *this;
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
      : File{path, 0, Mode::input}
    {
    }

    inline OutputFile::OutputFile(char const * path)
      : File{path, 0, Mode::output}
    {
    }

    constexpr int OutputFile::buildPerms_inner(Permissions permission) noexcept
    {
        return static_cast<std::underlying_type_t<Permissions>>(permission);
    }

    template <typename... PERMISSIONS>
    constexpr int
    OutputFile::buildPerms_inner(Permissions permission,
                                 PERMISSIONS... permissions) noexcept
    {
        return buildPerms_inner(permission) | buildPerms_inner(permissions...);
    }

    template <typename... PERMISSIONS>
    constexpr int
    OutputFile::buildPermissions(PERMISSIONS... permissions) noexcept
    {
        return buildPerms_inner(permissions...);
    }

    template <typename... PERMISSIONS>
    inline OutputFile OutputFile::createFile(char const * path,
                                             PERMISSIONS... perms)
    {
        return OutputFile{path, buildPermissions(perms...), Mode::create};
    }

    template <typename... MODES>
    inline OutputFile::OutputFile(char const * path, int permissions,
                                  MODES... modes)
      : File{path, permissions, Mode::output, modes...}
    {
    }

    template <typename BYTE>
    inline std::size_t InputFile::read(BYTE * data, std::size_t length)
    {
        static_assert(sizeof(BYTE) == 1, "BYTE type is not a byte");

        auto ret = ::read(fd(), data, length);
        if(ret == -1)
        {
            auto error = errno;
            throw std::runtime_error{::strerror(error)};
        }
        return ret;
    }

    template <typename BYTE>
    inline std::size_t OutputFile::write(BYTE const * data, std::size_t length)
    {
        static_assert(sizeof(BYTE) == 1, "BYTE type is not a byte");

        std::size_t wrote = 0;
        while(length > 0)
        {
            auto ret = ::write(fd(), data + wrote, length);
            if(ret == -1)
            {
                auto error = errno;
                throw std::runtime_error{::strerror(error)};
            }
            wrote += ret;
            length -= ret;
        }
        return wrote;
    }
}

#endif
