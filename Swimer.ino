#include "src/display.hpp"
#include "src/autom.hpp"
#include "src/vues.hpp"

swimer::Display display;
swimer::Input input;
swimer::Autom autom;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // TODO get input
  input.is_main_button_pushed = false;
  input.time_in_ms = millis();

  autom.crank(input);
  swimer::computeGraphics(display, input, autom.get());
  // TODO show display

  // blink
  if ((input.time_in_ms / 1000) % 2 == 0)
    digitalWrite(LED_BUILTIN, HIGH);
  else
    digitalWrite(LED_BUILTIN, LOW);
}
