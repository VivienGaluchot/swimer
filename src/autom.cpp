#include "autom.hpp"
#include <string.h>
#include <assert.h>


swimer::Input::Input()
    : is_main_button_pushed(false)
    , time_in_ms(0) {}


swimer::Output::Output()
    : lap_count(0)
    , run_time_in_ms(0)
    , state(PAUSE) {}


swimer::Autom::Autom()
    : m_output()
    , m_last_input() {}

void swimer::Autom::crank(const swimer::Input& input) {
    bool is_main_button_impulse = input.is_main_button_pushed && !m_last_input.is_main_button_pushed;
    
    if (m_output.state == RUN) {
        m_output.run_time_in_ms += input.time_in_ms - m_last_input.time_in_ms;
    }

    if (is_main_button_impulse) {
        if (m_output.state == RUN) {
            m_output.lap_count++;
        } else {
            m_output.state = RUN;
        }
        m_main_button_push_begin = input.time_in_ms;
    }

    if (input.is_main_button_pushed && m_output.state == RUN) {
        uint64_t main_button_maintain_time = input.time_in_ms - m_main_button_push_begin;
        if (main_button_maintain_time > MAINTAIN_TIME_FOR_PAUSE_IN_MS) {
            m_output.run_time_in_ms -= main_button_maintain_time;
            m_output.state = PAUSE;
        }
    }

    m_last_input = input;
}

const swimer::Output& swimer::Autom::get() const {
    return m_output;
}