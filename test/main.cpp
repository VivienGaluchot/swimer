#include <iostream>
#include "../src/display.hpp"
#include "stub.hpp"


swimer::Display display;

void setup() {
    std::cout << "setup" << std::endl;
}

void loop() {
    std::cout  << std::endl << "loop" << std::endl;
    std::cout << "--------------------------------" << std::endl;
    bool main_button = getMainButtonState();
    std::cout << std::endl;
    std::cout << "-inputs-------------------------" << std::endl;
    std::cout << "main_button " << main_button << std::endl;
    std::cout << std::endl;
    std::cout << "-screen-------------------------" << std::endl;
    show(display);
    std::cout << "--------------------------------" << std::endl;

    delay(1000);
}


int main(int arc, char** argv) {
    setup();
    while(true) {
        loop();
    }
    return 1;
}