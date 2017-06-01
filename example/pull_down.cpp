#include <cstdio>

#include <unistd.h>

#include <phantompi/sysfs/phantompi.hpp>
#include <phantompi/button.hpp>

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
    auto button = makePullDownButton(buttonGpio);

    char const * const messages[] = {
        "not pressed",
        "pressed"
    };
    while(count > 0)
    {
        usleep(500000);
        auto const state = button.getState();
        std::printf("Button is %s\n", messages[static_cast<std::underlying_type_t<phantompi::ButtonState>>(state)]);
        --count;
    }
    return 0;
}
