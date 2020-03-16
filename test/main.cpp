#include <iostream>
#include "../src/display.hpp"
#include "stub.hpp"


swimer::Display display;

void setup() {
    std::cout << "setup" << std::endl;
}

void loop() {
    std::cout << "loop" << std::endl;
    show(display);
    delay(1000);
}


int main(int arc, char** argv) {
    setup();
    while(true) {
        loop();
    }
    return 1;
}