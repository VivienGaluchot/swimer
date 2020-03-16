#ifndef STUB_HPP
#define STUB_HPP

#include <unistd.h>

void delay(int ms) {
    usleep(1000 * ms);
}

void show(const swimer::Display& display) {
    for (uint8_t y = 0; y < swimer::HEIGHT; y++) {
        for (uint8_t x = 0; x < swimer::WIDTH; x++) {
            swimer::Pixel pixel = display.getPixel(x, y);
            std::cout << int(pixel.intensity) << " ";
        }
        std::cout << "\n";
    }
}

#endif
