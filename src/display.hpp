#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <stdint.h>

namespace swimer {

const uint8_t WIDTH = 16;
const uint8_t HEIGHT = 9;

struct Pixel {
    uint8_t intensity;
};

class Display {
public:
    Display();
    Pixel& getPixel(uint8_t x, uint8_t y);
    const Pixel& getPixel(uint8_t x, uint8_t y) const;
    void setPixel(uint8_t x, uint8_t y, const Pixel& value);
private:
    Pixel m_pixels[WIDTH][HEIGHT];
};

}

#endif
