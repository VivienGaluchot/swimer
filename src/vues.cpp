#include "vues.hpp"
#include <string.h>
#include <assert.h>
#include <math.h>

namespace {

int16_t abs(int16_t a) {
    if (a >= 0)
        return a;
    else
        return -1 * a;
}

class Vect2D {
public:
    Vect2D(int16_t x, int16_t y)
    : x(x)
    , y(y) {}
    
    Vect2D(const Vect2D& p)
    : x(p.x)
    , y(p.y) {}

    void add(const Vect2D& p) {
        x += p.x;
        y += p.y;
    }

    void rem(const Vect2D& p) {
        x -= p.x;
        y -= p.y;
    }

    void rot(const float teta) {
        float ca = cos(teta);
        float sa = sin(teta);
        int16_t nx = round(ca * x - sa * y);
        y = round(sa * x + ca * y);
        x = nx;
    }

    int16_t x;
    int16_t y;
};

class GraphicContext {
public:
    GraphicContext(swimer::Display& display)
    : m_intensity(64)
    , m_cursor(0, 0)
    , m_display(display) {}

    GraphicContext(const GraphicContext& ctx)
    : m_intensity(ctx.m_intensity)
    , m_cursor(ctx.m_cursor)
    , m_display(ctx.m_display) {}

    void setCursor(const Vect2D& p) {
        m_cursor = p;
    }

    void translateCursor(const Vect2D& t) {
        m_cursor.add(t);
    }

    void setIntensity(uint8_t intensity) {
        m_intensity = intensity;
    }

    void setPixel(Vect2D p) {
        p.add(m_cursor);
        if (0 <= p.x && p.x < swimer::WIDTH && 0 <= p.y && p.y < swimer::HEIGHT) {
            m_display.setPixel(p.x, swimer::HEIGHT - p.y - 1, m_intensity);
        }
    }

private:
    uint8_t m_intensity;
    Vect2D m_cursor;
    swimer::Display& m_display;
};

class Widget {
public:
    Widget() {}
    virtual void render(GraphicContext& ctx) {}
};

class Line : public Widget {
public:
    Line(Vect2D a, Vect2D b)
        : m_a(a)
        , m_b(b) {}
    
    void render(GraphicContext& ctx) override {
        if (abs(m_b.y - m_a.y) < abs(m_b.x - m_a.x)) {
            if (m_a.x > m_b.x) {
                renderLow(ctx, m_b, m_a);
            } else {
                renderLow(ctx, m_a, m_b);
            }
        } else {
            if (m_a.y > m_b.y) {
                renderHigh(ctx, m_b, m_a);
            } else {
                renderHigh(ctx, m_a, m_b);
            }
        }
    }
    
private:
    void renderLow(GraphicContext& ctx, const Vect2D& a, const Vect2D& b) const {
        int16_t dx = b.x - a.x;
        int16_t dy = b.y - a.y;
        int16_t yi = 1;
        if (dy < 0) {
            yi = -1;
            dy = -1 * dy;
        }
        int16_t D = 2 * dy - dx;
        int16_t y = a.y;
        for (int16_t x = a.x; x <= b.x; x++) {
            ctx.setPixel(Vect2D(x, y));
            if (D > 0) {
                y = y + yi;
                D = D - 2 * dx;
            }
            D = D + 2 * dy;
        }
    }

    void renderHigh(GraphicContext& ctx, const Vect2D& a, const Vect2D& b) const {
        int16_t dx = b.x - a.x;
        int16_t dy = b.y - a.y;
        int16_t xi = 1;
        if (dx < 0) {
            xi = -1;
            dx = -1 * dx;
        }
        int16_t D = 2 * dx - dx;
        int16_t x = a.x;
        for (int16_t y = a.y; y <= b.y; y++) {
            ctx.setPixel(Vect2D(x, y));
            if (D > 0) {
                x = x + xi;
                D = D - 2 * dy;
            }
            D = D + 2 * dx;
        }
    }

    Vect2D m_a;
    Vect2D m_b;
};

class CharWidget : public Widget {
public:
    CharWidget(const char character)
    : m_character(character) {}
    
    void render(GraphicContext& ctx) override {
        if (m_character == 'P') {
            ctx.setPixel(Vect2D(0, 4));
            ctx.setPixel(Vect2D(0, 3));
            ctx.setPixel(Vect2D(0, 2));
            ctx.setPixel(Vect2D(0, 1));
            ctx.setPixel(Vect2D(0, 0));
            ctx.setPixel(Vect2D(1, 4));
            ctx.setPixel(Vect2D(1, 2));
            ctx.setPixel(Vect2D(2, 4));
            ctx.setPixel(Vect2D(2, 3));
            ctx.setPixel(Vect2D(2, 2));
            ctx.translateCursor(Vect2D(4, 0));
        }
    }
private:
    char m_character;
};

}

// public

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
        ctx.setCursor(::Vect2D(0, 4));
        CharWidget letter('P');
        letter.render(ctx);
    } else {
        ctx.setCursor(::Vect2D(0, 0));
        ctx.setIntensity(16);

        float sec_slider = (input.time_in_ms % 1000) / 1000.0;
        {            
            ::Vect2D center(4, HEIGHT / 2);
            ::Vect2D clock(0, 3);
            clock.rot(sec_slider * M_PI * -2);
            clock.add(center);

            Line l(center, clock);
            l.render(ctx);
            
            GraphicContext lctx(ctx);
            lctx.setIntensity(2);
            for (uint8_t a = 0; a < 8; a++) {
                ::Vect2D dot(0, 3);
                dot.rot(a * M_PI / 4);
                dot.add(center);
                lctx.setPixel(dot);
            }
        }

        if (output.pause_hold_ratio != 0) {
            float slider = WIDTH * output.pause_hold_ratio;
            for (uint8_t i = 0; i < slider; i++)
                ctx.setPixel(::Vect2D(i, 8));
            ctx.setIntensity(16 * (slider - uint8_t(slider)));
            ctx.setPixel(::Vect2D(slider, 8));
        }
    }
}