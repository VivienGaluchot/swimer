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


swimer::ImpulseFilter::ImpulseFilter()
    : m_counter(0)
    , m_state(false)
    , m_impulse(false) {}


void swimer::ImpulseFilter::crank(bool input) {
    // evolve counter
    if (input) {
        if (m_counter < swimer::IMPULSE_TRIGGER_COUNT) {
            m_counter++;
        }
    } else {
        if (m_counter >= 1) {
            m_counter--;
        }
    }

    // compute next state based on counter
    bool next_state = m_state;
    if (m_counter == 0) {
        next_state = false;
    } else if (m_counter == swimer::IMPULSE_TRIGGER_COUNT) {
        next_state = true;
    }

    // impulse when state is going up
    if (next_state != m_state) {
        m_impulse = !m_state && next_state;
    } else {
        m_impulse = false;
    }
    m_state = next_state;
}

bool swimer::ImpulseFilter::get() const {
    return m_impulse;
}

swimer::Autom::Autom()
    : m_output()
    , m_last_input()
    , m_state_begin(0)
    , m_ctr_button_filter()
    , m_pause_button_filter() {}

void swimer::Autom::crank(const swimer::Input& input) {
    m_ctr_button_filter.crank(input.is_ctr_button_pushed);
    m_pause_button_filter.crank(input.is_pause_button_pushed);

    bool is_ctr_button_impulse = m_ctr_button_filter.get();
    bool is_pause_button_impulse = m_pause_button_filter.get();
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