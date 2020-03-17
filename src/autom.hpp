#ifndef AUTOM_HPP
#define AUTOM_HPP

#include <stdint.h>

namespace swimer {

struct Inputs {
    bool main_button;
    // TODO
}

struct Outputs {
    // TODO
}

class Autom {
public:
    Autom();
    void step(const Inputs& inputs);
    const Outputs& get() const;
private:
    // TODO
}

}

#endif
