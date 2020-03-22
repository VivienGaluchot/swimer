#include "vues.hpp"
#include <string.h>
#include <assert.h>

void swimer::computeGraphics(Display& display, const Input& input, const Output& output) {
    for (uint8_t x = 0; x < WIDTH; x++) {
        for (uint8_t y = 0; y < HEIGHT; y++) {
            // clean
            display.setPixel(x, y, 0);

            // blink background
            if ((input.time_in_ms / 1000) % 2 == 0) {
                display.setPixel(x, y, 1);
            }

            // borders
            if (input.is_main_button_pushed) {
                if ((y == 0 || y == HEIGHT - 1) || (x == 0 || x == WIDTH - 1)) {
                    display.setPixelIfLower(x, y, 16);
                }
            }
        }
    }
}