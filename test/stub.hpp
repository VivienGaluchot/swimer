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

bool getMainButtonState() {
    std::string is_on;
    std::cout << "PAUSE : is main button on ? (y/n)" << std::endl;
    std::cin >> is_on;
    return is_on == "y";
}

#endif
