#ifndef AUTOM_HPP
#define AUTOM_HPP

#include <stdint.h>

namespace swimer {

const uint16_t MAINTAIN_TIME_FOR_PAUSE_IN_MS = 3000;
const uint8_t IMPULSE_TRIGGER_COUNT = 5;
const uint16_t MIN_CTR_IMPULSE_PERIOD_IN_MS = 500;

enum State { PAUSE, RUN };

struct Input {
    Input();
    bool is_ctr_button_pushed;
    bool is_pause_button_pushed;
    uint64_t time_in_ms;
};

struct Output {
    Output();
    uint16_t lap_count;
    uint64_t run_time_in_ms;
    State state;
    uint64_t state_time_in_ms;
};

class ImpulseFilter {
public:
    ImpulseFilter();
    void crank(bool input);
    bool get() const;
private:
    uint8_t m_counter;
    bool m_state;
    bool m_impulse;
};

class Autom {
public:
    Autom();
    void crank(const Input& input);
    const Output& get() const;
private:
    Output m_output;
    Input m_last_input;
    uint64_t m_state_begin;
    ImpulseFilter m_ctr_button_filter;
    ImpulseFilter m_pause_button_filter;
    uint64_t m_last_ctr_button_impulse_time_in_ms;
};

}

#endif
