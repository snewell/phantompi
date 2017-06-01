#include <cstdio>

#include <unistd.h>

#include <phantompi/sysfs/phantompi.hpp>

int main(int argc,
         char ** argv)
{
    if(argc != 3)
    {
        std::printf("Usage: %s <gpio> <count>\n", argv[0]);
        return (argc == 1);
    }

    auto gpio = std::atoi(argv[1]);
    auto count = std::atoi(argv[2]);

    auto buttonGpio = phantompi::sysfs::accessInputGpio(gpio);

    char const * const messages[] = {
        "pressed",
        "not pressed"
    };
    while(count > 0)
    {
        usleep(500000);
        auto const state = buttonGpio->getState();
        std::printf("Button is %s\n", messages[static_cast<std::underlying_type_t<phantompi::sysfs::Gpio::State>>(state)]);
        --count;
    }
    return 0;
}
