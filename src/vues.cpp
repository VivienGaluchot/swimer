#include "vues.hpp"
#include <string.h>
#include <assert.h>

void swimer::computeGraphics(Display& display, const Input& input, const Output& output) {
    Pixel all;
    if (input.is_main_button_pushed) {
        all.intensity = 1;
    } else {
        all.intensity = 0;
    }
    for (uint8_t x = 0; x < WIDTH; x++) {
        for (uint8_t y = 0; y < HEIGHT; y++) {
            display.setPixel(x, y, all);
        }
    }
}