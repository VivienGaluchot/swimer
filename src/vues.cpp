#include "vues.hpp"
#include <string.h>
#include <assert.h>

namespace {

class GraphicContext {
public:
    GraphicContext(swimer::Display& display)
    : m_intensity(64)
    , m_cursor_x(0)
    , m_cursor_y(0)
    , m_display(display) {}

    void setCursor(int16_t x, int16_t y) {
        m_cursor_x = x;
        m_cursor_y = y;
    }

    void translateCursor(int16_t x, int16_t y) {
        m_cursor_x += x;
        m_cursor_y += y;
    }

    void setIntensity(uint8_t intensity) {
        m_intensity = intensity;
    }

    void setPixel(int16_t x, int16_t y) {
        x += m_cursor_x;
        y += m_cursor_y;
        if (0 <= x && x < swimer::WIDTH && 0 <= y && y < swimer::HEIGHT) {
            m_display.setPixel(x, swimer::HEIGHT - y - 1, m_intensity);
        }
    }

private:
    uint8_t m_intensity;
    int16_t m_cursor_x;
    int16_t m_cursor_y;
    swimer::Display& m_display;
};

class Widget {
public:
    Widget() {}
    virtual void render(GraphicContext& ctx) {}
};

class CharWidget : public Widget {
public:
    CharWidget(const char character)
    : m_character(character) {}
    
    virtual void render(GraphicContext& ctx) {
        if (m_character == 'P') {
            ctx.setPixel(0, 4);
            ctx.setPixel(0, 3);
            ctx.setPixel(0, 2);
            ctx.setPixel(0, 1);
            ctx.setPixel(0, 0);
            ctx.setPixel(1, 4);
            ctx.setPixel(1, 2);
            ctx.setPixel(2, 4);
            ctx.setPixel(2, 3);
            ctx.setPixel(2, 2);
            ctx.translateCursor(4, 0);
        }
    }
private:
    char m_character;
};

}

void swimer::computeGraphics(Display& display, const Input& input, const Output& output) {
    for (uint8_t x = 0; x < WIDTH; x++) {
        for (uint8_t y = 0; y < HEIGHT; y++) {
            // clean
            if (input.is_main_button_pushed) {
                display.setPixel(x, y, 1);
            } else {
                display.setPixel(x, y, 0);
            }
        }
    }
    
    GraphicContext ctx(display);
    ctx.setIntensity(16);

    if (output.state == PAUSE) {
        ctx.setCursor(0, 4);
        CharWidget letter('P');
        letter.render(ctx);
    } else {
        ctx.setCursor(0, 0);
        ctx.setIntensity(16);
        uint16_t slider = WIDTH * (input.time_in_ms % 1000) / 1000;
        ctx.setPixel(slider, 0);
        if (output.pause_hold_ratio != 0) {
            float slider = WIDTH * output.pause_hold_ratio;
            for (uint8_t i = 0; i < slider; i++)
                ctx.setPixel(i, 8);
            ctx.setIntensity(16 * (slider - uint8_t(slider)));
            ctx.setPixel(slider, 8);
        }
    }
}