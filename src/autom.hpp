#ifndef AUTOM_HPP
#define AUTOM_HPP

#include <stdint.h>

namespace swimer {

const uint16_t MAINTAIN_TIME_FOR_PAUSE_IN_MS = 5000;

enum State { PAUSE, RUN };

struct Input {
    Input();
    bool is_main_button_pushed;
    uint64_t time_in_ms;
};

struct Output {
    Output();
    uint16_t lap_count;
    uint64_t run_time_in_ms;
    State state;
};

class Autom {
public:
    Autom();
    void crank(const Input& input);
    const Output& get() const;
private:
    Output m_output;
    Input m_last_input;
    uint64_t m_main_button_push_begin;
};

}

#endif
