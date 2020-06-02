#include "autom.hpp"
#include <string.h>
#include <assert.h>


swimer::Input::Input()
    : is_ctr_button_pushed(false)
    , is_pause_button_pushed(false)
    , time_in_ms(0) {}


swimer::Output::Output()
    : lap_count(0)
    , run_time_in_ms(0)
    , state(PAUSE)
    , state_time_in_ms(0) {}


swimer::Autom::Autom()
    : m_output()
    , m_last_input()
    , m_state_begin(0) {}

void swimer::Autom::crank(const swimer::Input& input) {
    bool is_ctr_button_impulse = input.is_ctr_button_pushed && !m_last_input.is_ctr_button_pushed;
    bool is_pause_button_impulse = input.is_pause_button_pushed && !m_last_input.is_pause_button_pushed;
    State next_state = m_output.state;
    
    if (m_output.state == RUN) {
        m_output.run_time_in_ms += input.time_in_ms - m_last_input.time_in_ms;

        if (is_ctr_button_impulse) {
            m_output.lap_count++;
        }
        if (is_pause_button_impulse) {
            next_state = PAUSE;
        }
    } else {
        if (is_ctr_button_impulse || is_pause_button_impulse) {
            next_state = RUN;
        }
    }

    if (next_state != m_output.state) {
        m_output.state = next_state;
        m_state_begin = input.time_in_ms;
    }
    m_output.state_time_in_ms = input.time_in_ms - m_state_begin;
    m_last_input = input;
}

const swimer::Output& swimer::Autom::get() const {
    return m_output;
}