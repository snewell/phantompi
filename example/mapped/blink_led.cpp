#include <cstdio>
#include <cstdlib>

#include <unistd.h>

#include <phantompi/led.hpp>
#include <phantompi/mapped/phantompi.hpp>

int main(int argc, char ** argv)
{
    if(argc != 3)
    {
        std::printf("Usage: %s <gpio> <count>\n", argv[0]);
        return static_cast<int>(argc == 1);
    }

    auto gpio = std::atoi(argv[1]);
    auto count = std::atoi(argv[2]);

    auto ledGpio = phantompi::mapped::accessOutputGpio(gpio);
    auto led = makeLed(ledGpio);

    while(count > 0)
    {
        led.turnOn();
        usleep(100000);
        led.turnOff();
        usleep(100000);
        --count;
    }
    return 0;
}