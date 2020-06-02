#ifndef STUB_HPP
#define STUB_HPP

#include <unistd.h>
#include <stdint.h>
#include <time.h>

const uint64_t NS_PER_MS = 1000000;
const uint64_t MS_PER_S = 1000;

void delay(int ms) {
    usleep(1000 * ms);
}

uint64_t msTime() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC , &ts);
    return ts.tv_nsec / NS_PER_MS + ts.tv_sec * MS_PER_S;
}

void show(const swimer::Display& display) {
    for (uint8_t y = 0; y < swimer::HEIGHT; y++) {
        for (uint8_t x = 0; x < swimer::WIDTH; x++) {
            swimer::Pixel pixel = display.getPixel(x, y);
            if (pixel.intensity == 0) {
                std::cout << " ";
            } else if (pixel.intensity < 16) {
                std::cout << "-";
            } else if (pixel.intensity < 62) {
                std::cout << "+";
            } else {
                std::cout << "#";
            }
            std::cout << " ";
        }
        std::cout << "\n";
    }
}

bool isCtrButtonPushed() {
    std::string is_on;
    std::cout << "PAUSE : is main button pushed ? (y/n)" << std::endl;
    std::cin >> is_on;
    return is_on == "y";
}

bool isPauseButtonPushed() {
    std::string is_on;
    std::cout << "PAUSE : is main pause pushed ? (y/n)" << std::endl;
    std::cin >> is_on;
    return is_on == "y";
}

#endif
