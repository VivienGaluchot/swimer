#include "src/display.hpp"
#include "src/autom.hpp"
#include "src/vues.hpp"

#include <Adafruit_IS31FL3731.h>

const uint8_t MAIN_BUTTON = 2;
Adafruit_IS31FL3731 matrix = Adafruit_IS31FL3731();

swimer::Display disp;
swimer::Input input;
swimer::Autom autom;

void setup() {
  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(MAIN_BUTTON, INPUT_PULLUP);
  
  if (!matrix.begin()) {
    Serial.println("IS31 not found");
    while(1);
  }
  matrix.clear();
}

void loop() {
  // get input
  input.is_main_button_pushed = !digitalRead(MAIN_BUTTON);
  input.time_in_ms = millis();

  /* Serial.print("main button : ");
  Serial.print(input.is_main_button_pushed, DEC);
  Serial.print(", time : ");
  Serial.print(int (input.time_in_ms), DEC);
  Serial.println(); */

  // compute cycle
  autom.crank(input);
  swimer::computeGraphics(disp, input, autom.get());
  
  // show display
  for (uint8_t y = 0; y < swimer::HEIGHT; y++) {
      for (uint8_t x = 0; x < swimer::WIDTH; x++) {
          swimer::Pixel& pixel = disp.getPixel(x, y);
          if (pixel.intensity != pixel.last_intensity) {
             matrix.drawPixel(x, y, pixel.intensity);
             pixel.last_intensity = pixel.intensity;
          }
      }
  }
  
  // blink
  if ((input.time_in_ms / 1000) % 2 == 0) {
     digitalWrite(LED_BUILTIN, HIGH);
  } else {
     digitalWrite(LED_BUILTIN, LOW);
  }
}
