#include "display.hpp"
#include <string.h>
#include <assert.h>


swimer::Display::Display()
    :  m_pixels() {
    memset(&m_pixels, 0, sizeof(m_pixels));
}

swimer::Pixel& swimer::Display::getPixel(uint8_t x, uint8_t y) {
    assert(x < swimer::WIDTH);
    assert(y < swimer::HEIGHT);
    return m_pixels[x][y];
}

const swimer::Pixel& swimer::Display::getPixel(uint8_t x, uint8_t y) const {
    assert(x < swimer::WIDTH);
    assert(y < swimer::HEIGHT);
    return m_pixels[x][y];
}

void swimer::Display::setPixel(uint8_t x, uint8_t y, const uint8_t& intensity) {
    assert(x < swimer::WIDTH);
    assert(y < swimer::HEIGHT);
    m_pixels[x][y].intensity = intensity;
}

void swimer::Display::setPixelIfLower(uint8_t x, uint8_t y, const uint8_t& intensity) {
    assert(x < swimer::WIDTH);
    assert(y < swimer::HEIGHT);
    if (m_pixels[x][y].intensity < intensity) {
        setPixel(x, y, intensity);
    }
}
