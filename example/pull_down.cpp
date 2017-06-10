#include <cstdio>

#include <unistd.h>

#include <phantompi/button.hpp>
#include <phantompi/sysfs/phantompi.hpp>

#include "button_example.hpp"

int main(int argc, char ** argv)
{
    if(argc != 3)
    {
        std::printf("Usage: %s <gpio> <count>\n", argv[0]);
        return static_cast<int>(argc == 1);
    }

    auto gpio = std::atoi(argv[1]);
    auto count = std::atoi(argv[2]);

    auto buttonGpio = phantompi::sysfs::accessInputGpio(gpio);
    auto button = makePullDownButton(buttonGpio);

    while(count > 0)
    {
        usleep(500000);
        checkButton(button);
        --count;
    }
    return 0;
}
