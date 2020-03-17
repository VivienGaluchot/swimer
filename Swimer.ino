#include "src/display.hpp"
#include "src/autom.hpp"

swimer::Display display;
swimer::Input input;
swimer::Autom autom;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);

  // TODO get input
  autom.crank(input);
  // TODO use output
}
