#include <iostream>
#include "../src/autom.hpp"
#include "../src/display.hpp"
#include "stub.hpp"


swimer::Display display;
swimer::Input input;
swimer::Autom autom;

void setup() {
    std::cout << "setup" << std::endl;
}

void getInput(swimer::Input& input) {
    input.is_main_button_pushed = isMainButtonPushed();
    input.time_in_ms = msTime();
}

void loop() {
    std::cout << std::endl << "loop" << std::endl;
    std::cout << std::endl;

    std::cout << "-input--------------------------" << std::endl;
    getInput(input);
    std::cout << std::endl;
    std::cout << "main_button : " << input.is_main_button_pushed << std::endl;
    std::cout << "time_in_ms  : " << input.time_in_ms << std::endl;
    std::cout << std::endl;

    std::cout << "-autom--------------------------" << std::endl;
    autom.crank(input);
    std::cout << std::endl;
    std::cout << "lap_count : " << autom.get().lap_count << std::endl;
    std::cout << "state     : " << ((autom.get().state == swimer::PAUSE) ? "PAUSE" : "RUN") << std::endl;
    std::cout << "run_time  : " << autom.get().run_time_in_ms << std::endl;
    std::cout << std::endl;

    std::cout << "-screen-------------------------" << std::endl;
    show(display);
    std::cout << "--------------------------------" << std::endl;

    delay(100);
}


int main(int arc, char** argv) {
    setup();
    while(true) {
        loop();
    }
    return 1;
}