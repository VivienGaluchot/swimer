#include "vues.hpp"
#include <string.h>
#include <assert.h>
#include <math.h>
#include <stdio.h>


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

    Vect2D getCursor() const {
        return m_cursor;
    }

    void translateCursor(const Vect2D& t) {
        m_cursor.add(t);
    }

    void setIntensity(uint8_t intensity) {
        m_intensity = intensity;
    }

    void setPixel(Vect2D p, float relative_instensity=1.) {
        p.add(getCursor());
        if (0 <= p.x && p.x < swimer::WIDTH && 0 <= p.y && p.y < swimer::HEIGHT) {
            m_display.setPixel(p.x, swimer::HEIGHT - p.y - 1, m_intensity * relative_instensity);
        }
    }

private:
    uint8_t m_intensity;
    Vect2D m_cursor;
    swimer::Display& m_display;
};


class RollContext : public GraphicContext {
public:
    /**
     * spaces
     * |      |        roll width
     * |      :    |   context width
     *        |    |   max offset
     * 
     * times of annimation vs offet
     *  |             .---
     *  |          ../
     *  |       ../
     *  |  ----/
     *    Fix   Roll    Fix
     */
    RollContext(const GraphicContext& ctx,
                uint64_t fix_time_in_ms,
                uint64_t roll_time_in_ms,
                uint64_t time_in_ms,
                uint16_t roll_width,
                uint16_t context_width)
        : GraphicContext(ctx) {
            uint16_t max_offset = 0;
            if (context_width > roll_width) {
                max_offset = context_width - roll_width;
            }

            int16_t time_slice_in_ms = time_in_ms % (2 * fix_time_in_ms + roll_time_in_ms);
            if (time_slice_in_ms <= fix_time_in_ms) {
                // fix start
            } else if (time_slice_in_ms >= fix_time_in_ms + roll_time_in_ms) {
                // fix end
                translateCursor(Vect2D(-1 * max_offset, 0));
            } else {
                // ramp up
                float anim_ratio = (time_slice_in_ms - fix_time_in_ms) / float (roll_time_in_ms);
                int16_t offset = round((max_offset) * anim_ratio);
                translateCursor(Vect2D(-1 * offset, 0));
            }
        }
};


class Widget {
public:
    Widget() {}
    virtual void render(GraphicContext& ctx) {}
};


class Segment : public Widget {
public:
    Segment(Vect2D a, Vect2D b)
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
    CharWidget(const char character, bool big = false)
        : m_character(character)
        , m_big(big) {}

    uint16_t getLenght() const {
        switch (m_character) {
            case 'P':
                return m_big ? 0 : 3;
            case '0':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                return m_big ? 4 : 3;
            case '1':
                return m_big ? 3 : 2;
            case ' ':
            case ':':
            case '.':
                return 1;
            default:
                return 0;
        }
    }
    
    void render(GraphicContext& ctx) override {
        if (!m_big) {
            if (m_character == 'P') {
                Segment(Vect2D(0, 4), Vect2D(0, 0)).render(ctx);
                Segment(Vect2D(0, 4), Vect2D(2, 4)).render(ctx);
                Segment(Vect2D(0, 2), Vect2D(2, 2)).render(ctx);
                Segment(Vect2D(2, 2), Vect2D(2, 4)).render(ctx);
            } else if (m_character == ':') {
                ctx.setPixel(Vect2D(0, 1));
                ctx.setPixel(Vect2D(0, 3));
            } else if (m_character == '.') {
                ctx.setPixel(Vect2D(0, 0));
            } else if (m_character == '0') {
                Segment(Vect2D(0, 4), Vect2D(2, 4)).render(ctx);
                Segment(Vect2D(2, 4), Vect2D(2, 0)).render(ctx);
                Segment(Vect2D(0, 4), Vect2D(0, 0)).render(ctx);
                Segment(Vect2D(0, 0), Vect2D(2, 0)).render(ctx);
            } else if (m_character == '1') {
                Segment(Vect2D(1, 4), Vect2D(1, 0)).render(ctx);
                Segment(Vect2D(1, 4), Vect2D(0, 3)).render(ctx);
            } else if (m_character == '2') {
                Segment(Vect2D(0, 4), Vect2D(2, 4)).render(ctx);
                Segment(Vect2D(2, 4), Vect2D(2, 2)).render(ctx);
                Segment(Vect2D(2, 2), Vect2D(0, 2)).render(ctx);
                Segment(Vect2D(0, 2), Vect2D(0, 0)).render(ctx);
                Segment(Vect2D(0, 0), Vect2D(2, 0)).render(ctx);
            } else if (m_character == '3') {
                Segment(Vect2D(0, 4), Vect2D(2, 4)).render(ctx);
                Segment(Vect2D(2, 4), Vect2D(2, 0)).render(ctx);
                Segment(Vect2D(2, 0), Vect2D(0, 0)).render(ctx);
                Segment(Vect2D(1, 2), Vect2D(2, 2)).render(ctx);
            } else if (m_character == '4') {
                Segment(Vect2D(2, 4), Vect2D(0, 2)).render(ctx);
                Segment(Vect2D(0, 1), Vect2D(2, 1)).render(ctx);
                Segment(Vect2D(2, 4), Vect2D(2, 0)).render(ctx);
            } else if (m_character == '5') {
                Segment(Vect2D(0, 4), Vect2D(2, 4)).render(ctx);
                Segment(Vect2D(0, 4), Vect2D(0, 2)).render(ctx);
                Segment(Vect2D(0, 2), Vect2D(1, 2)).render(ctx);
                Segment(Vect2D(2, 2), Vect2D(2, 0)).render(ctx);
                Segment(Vect2D(0, 0), Vect2D(1, 0)).render(ctx);
            } else if (m_character == '6') {
                Segment(Vect2D(0, 4), Vect2D(2, 4)).render(ctx);
                Segment(Vect2D(0, 4), Vect2D(0, 0)).render(ctx);
                Segment(Vect2D(0, 4), Vect2D(0, 2)).render(ctx);
                Segment(Vect2D(2, 2), Vect2D(0, 2)).render(ctx);
                Segment(Vect2D(2, 2), Vect2D(2, 0)).render(ctx);
                Segment(Vect2D(0, 0), Vect2D(2, 0)).render(ctx);
            } else if (m_character == '7') {
                Segment(Vect2D(0, 4), Vect2D(2, 4)).render(ctx);
                Segment(Vect2D(2, 4), Vect2D(2, 0)).render(ctx);
                ctx.setPixel(Vect2D(1, 2));
            } else if (m_character == '8') {
                Segment(Vect2D(0, 4), Vect2D(2, 4)).render(ctx);
                Segment(Vect2D(2, 4), Vect2D(2, 0)).render(ctx);
                Segment(Vect2D(0, 4), Vect2D(0, 0)).render(ctx);
                Segment(Vect2D(0, 4), Vect2D(0, 2)).render(ctx);
                Segment(Vect2D(2, 2), Vect2D(0, 2)).render(ctx);
                Segment(Vect2D(0, 0), Vect2D(2, 0)).render(ctx);
            } else if (m_character == '9') {
                Segment(Vect2D(0, 4), Vect2D(2, 4)).render(ctx);
                Segment(Vect2D(2, 4), Vect2D(2, 0)).render(ctx);
                Segment(Vect2D(0, 4), Vect2D(0, 2)).render(ctx);
                Segment(Vect2D(0, 4), Vect2D(0, 2)).render(ctx);
                Segment(Vect2D(2, 2), Vect2D(0, 2)).render(ctx);
                Segment(Vect2D(0, 0), Vect2D(2, 0)).render(ctx);
            }
        } else {
            if (m_character == ':') {
                ctx.setPixel(Vect2D(0, 1));
                ctx.setPixel(Vect2D(0, 3));
            } else if (m_character == '.') {
                ctx.setPixel(Vect2D(0, 0));
            } else if (m_character == '0') {
                Segment(Vect2D(1, 6), Vect2D(2, 6)).render(ctx);
                Segment(Vect2D(3, 5), Vect2D(3, 1)).render(ctx);
                Segment(Vect2D(0, 5), Vect2D(0, 1)).render(ctx);
                Segment(Vect2D(1, 0), Vect2D(2, 0)).render(ctx);
            } else if (m_character == '1') {
                Segment(Vect2D(2, 6), Vect2D(2, 0)).render(ctx);
                Segment(Vect2D(2, 6), Vect2D(0, 4)).render(ctx);
            } else if (m_character == '2') {
                Segment(Vect2D(3, 0), Vect2D(0, 0)).render(ctx);
                Segment(Vect2D(0, 1), Vect2D(3, 4)).render(ctx);
                Segment(Vect2D(3, 4), Vect2D(3, 5)).render(ctx);
                Segment(Vect2D(2, 6), Vect2D(1, 6)).render(ctx);
                Segment(Vect2D(0, 5), Vect2D(0, 4)).render(ctx);
            } else if (m_character == '3') {
                Segment(Vect2D(0, 0), Vect2D(2, 0)).render(ctx);
                Segment(Vect2D(3, 1), Vect2D(3, 2)).render(ctx);
                Segment(Vect2D(2, 3), Vect2D(1, 3)).render(ctx);
                Segment(Vect2D(3, 4), Vect2D(3, 5)).render(ctx);
                Segment(Vect2D(2, 6), Vect2D(0, 6)).render(ctx);
            } else if (m_character == '4') {
                Segment(Vect2D(3, 6), Vect2D(3, 0)).render(ctx);
                Segment(Vect2D(3, 6), Vect2D(0, 2)).render(ctx);
                Segment(Vect2D(0, 1), Vect2D(3, 1)).render(ctx);
            } else if (m_character == '5') {
                Segment(Vect2D(0, 6), Vect2D(3, 6)).render(ctx);
                Segment(Vect2D(0, 6), Vect2D(0, 3)).render(ctx);
                Segment(Vect2D(0, 3), Vect2D(2, 3)).render(ctx);
                Segment(Vect2D(3, 2), Vect2D(3, 1)).render(ctx);
                Segment(Vect2D(2, 0), Vect2D(0, 0)).render(ctx);
            } else if (m_character == '6') {
                Segment(Vect2D(1, 6), Vect2D(3, 6)).render(ctx);
                Segment(Vect2D(0, 5), Vect2D(0, 1)).render(ctx);
                Segment(Vect2D(0, 3), Vect2D(2, 3)).render(ctx);
                Segment(Vect2D(3, 2), Vect2D(3, 1)).render(ctx);
                Segment(Vect2D(2, 0), Vect2D(1, 0)).render(ctx);
            } else if (m_character == '7') {
                Segment(Vect2D(0, 6), Vect2D(3, 6)).render(ctx);
                Segment(Vect2D(3, 6), Vect2D(0, 0)).render(ctx);
                Segment(Vect2D(3, 6), Vect2D(0, 0)).render(ctx);
            } else if (m_character == '8') {
                Segment(Vect2D(0, 2), Vect2D(0, 1)).render(ctx);
                Segment(Vect2D(1, 0), Vect2D(2, 0)).render(ctx);
                Segment(Vect2D(3, 1), Vect2D(3, 2)).render(ctx);
                Segment(Vect2D(2, 3), Vect2D(1, 3)).render(ctx);
                Segment(Vect2D(3, 4), Vect2D(3, 5)).render(ctx);
                Segment(Vect2D(2, 6), Vect2D(1, 6)).render(ctx);
                Segment(Vect2D(0, 5), Vect2D(0, 4)).render(ctx);
            } else if (m_character == '9') {
                Segment(Vect2D(1, 6), Vect2D(2, 6)).render(ctx);
                Segment(Vect2D(3, 5), Vect2D(3, 1)).render(ctx);
                Segment(Vect2D(2, 0), Vect2D(0, 0)).render(ctx);
                Segment(Vect2D(0, 5), Vect2D(0, 4)).render(ctx);
                Segment(Vect2D(1, 3), Vect2D(2, 3)).render(ctx);
            }
        }
        ctx.translateCursor(Vect2D(getLenght(), 0));
    }

private:
    char m_character;
    bool m_big;
};


class StringWidget : public Widget {
public:
    StringWidget(const char* string, bool big=false)
        : m_string(string)
        , m_big(big) {}

    uint16_t getLenght() const {
        uint16_t length = 0;
        const char* i = m_string;
        bool first = true;
        while (*i != 0) {
            if (!first)
                length += 1;
            length += CharWidget(*i, m_big).getLenght();
            i++;
            first = false;
        }
        return length;
    }

    void render(GraphicContext& ctx) override {
        const char* i = m_string;
        bool first = true;
        while (*i != 0) {
            if (!first)
                ctx.translateCursor(Vect2D(1, 0));
            CharWidget(*i, m_big).render(ctx);
            i++;
            first = false;
        }
    }

private:
    const char* m_string;
    bool m_big;
};

}


// public

void swimer::computeGraphics(Display& display, const Input& input, const Output& output) {
    // clean
    for (uint8_t x = 0; x < WIDTH; x++) {
        for (uint8_t y = 0; y < HEIGHT; y++) {
            display.setPixel(x, y, 0);
        }
    }
    
    GraphicContext ctx(display);
    ctx.setCursor(::Vect2D(0, 0));
    ctx.setIntensity(32);

    if (output.state == PAUSE) {
        const uint64_t PAUSE_SYMBOL_TIME_IN_MS = 2000;
        const uint64_t FIX_TIME_IN_MS = 2000;
        const uint64_t ROLL_TIME_IN_MS = 3000;

        const uint64_t TOTAL_PERIOD_TIME_IN_MS = PAUSE_SYMBOL_TIME_IN_MS + 2 * FIX_TIME_IN_MS + ROLL_TIME_IN_MS;

        uint64_t period_time_in_ms = output.state_time_in_ms % TOTAL_PERIOD_TIME_IN_MS;
        if (period_time_in_ms < PAUSE_SYMBOL_TIME_IN_MS) {
            if (period_time_in_ms % 2000 >= 500 && period_time_in_ms % 2000 < 1500) {
                ctx.setCursor(::Vect2D(WIDTH / 2 - 2, 1));
                ::Segment(::Vect2D(0, 0), :: Vect2D(0, 6)).render(ctx);
                ::Segment(::Vect2D(1, 0), :: Vect2D(1, 6)).render(ctx);
                ::Segment(::Vect2D(3, 0), :: Vect2D(3, 6)).render(ctx);
                ::Segment(::Vect2D(4, 0), :: Vect2D(4, 6)).render(ctx);
            }
        } else {
            ctx.setCursor(::Vect2D(0, 2));
            char lap_count_str[16];
            uint16_t run_time_in_s = output.run_time_in_ms / 1000;
            uint16_t run_time_s = run_time_in_s % 60;
            uint16_t run_time_m = (run_time_in_s / 60) % 60;
            uint16_t run_time_h = (run_time_in_s / (60 * 60));
            if (run_time_h > 0) {
                snprintf(lap_count_str, sizeof(lap_count_str), "%u:%u:%u", run_time_h, run_time_m, run_time_s);
            } else if (run_time_m > 0) {
                uint16_t run_time_ms = output.run_time_in_ms % 1000;
                snprintf(lap_count_str, sizeof(lap_count_str), "%u:%u.%01u", run_time_m, run_time_s, run_time_ms / 100);
            } else {
                uint16_t run_time_ms = output.run_time_in_ms % 1000;
                snprintf(lap_count_str, sizeof(lap_count_str), "%u.%01u", run_time_s, run_time_ms / 100);
            }

            auto text = StringWidget(lap_count_str);
            auto length = text.getLenght();
            if (length > WIDTH) {
                auto r_ctx = RollContext(ctx,
                                        FIX_TIME_IN_MS,
                                        ROLL_TIME_IN_MS,
                                        period_time_in_ms - PAUSE_SYMBOL_TIME_IN_MS,
                                        WIDTH, text.getLenght());
                text.render(r_ctx);
            } else {
                ctx.setCursor(::Vect2D(WIDTH - length, 2));
                text.render(ctx);
            }
        }
    } else {
        // record dot
        if ((output.run_time_in_ms / 1000) % 2) {
            ctx.setCursor(::Vect2D(0, 2));
            Segment(::Vect2D(0, 1), ::Vect2D(1, 2)).render(ctx);
            Segment(::Vect2D(1, 0), ::Vect2D(0, 1)).render(ctx);
        }

        // pause holding bar
        if (output.pause_hold_ratio > 0.1) {
            auto i_ctx = GraphicContext(ctx);
            i_ctx.setCursor(::Vect2D(0, 0));
            float slider = WIDTH * output.pause_hold_ratio;
            for (uint8_t i = 0; i < slider; i++)
                i_ctx.setPixel(::Vect2D(i, 8));
            i_ctx.setIntensity(16 * (slider - uint8_t(slider)));
            i_ctx.setPixel(::Vect2D(slider, 8));
        }
        
        // counter
        char lap_count_str[8];
        snprintf(lap_count_str, sizeof(lap_count_str), "%u", output.lap_count);
        auto text = StringWidget(lap_count_str, true);
        ctx.setCursor(::Vect2D(WIDTH - text.getLenght(), 0));
        text.render(ctx);
    }
}