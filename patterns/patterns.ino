#include "FastLED.h"
#include "mixer.h"
#include "matrix.h"

// How many leds in your strip?
#define NUM_LEDS 74

#define DATA_PIN 8

// Define the array of leds
CRGB leds[NUM_LEDS];

void setup() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);

}


void update() {
  
}

void draw() {
  FastLED.show();
  delay(500);  
}

void loop() {
  FastLED.show();
  delay(500);
}
