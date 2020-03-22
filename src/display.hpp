#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <stdint.h>

namespace swimer {

const uint8_t WIDTH = 16;
const uint8_t HEIGHT = 9;
const uint8_t FRAME_COUNT = 2;

struct Pixel {
    uint8_t intensity;
    uint8_t last_intensity;
};

class Display {
public:
    Display();

    uint8_t getFrame() const;
    void setFrame(uint8_t frame);

    Pixel& getPixel(uint8_t x, uint8_t y);
    const Pixel& getPixel(uint8_t x, uint8_t y) const;
    void setPixel(uint8_t x, uint8_t y, const uint8_t& intensity);
    void setPixelIfLower(uint8_t x, uint8_t y, const uint8_t& intensity);
private:
    uint8_t m_frame;
    Pixel m_pixels[FRAME_COUNT][WIDTH][HEIGHT];
};

}

#endif
