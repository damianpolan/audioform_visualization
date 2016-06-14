#include "FastLED.h"
#include "mixer.h"
#include "matrix.h"
#include "point.h"
#include "shapes.h"
#include "tools.h"

// How many leds in your strip?
#define NUM_LEDS 74
#define BRIGHTNESS 10

#define DATA_PIN 8

int32_t matrix_right[8][11] = {
  {-1,-1,67,68,69,70,71,72,73,-1,-1},
  {-1,58,59,60,61,62,63,64,65,66,-1},
  {48,49,50,51,52,53,54,55,56,57,-1},
  {37,38,39,40,41,42,43,44,45,46,47},
  {26,27,28,29,30,31,32,33,34,35,36},
  {-1,16,17,18,19,20,21,22,23,24,25},
  {-1,-1,7,8,9,10,11,12,13,14,15},
  {-1,-1,-1,6,5,4,3,2,1,0,-1},
};

// Define the array of leds
CRGB leds[NUM_LEDS];

int16_t xy_to_pos(uint16_t x, uint16_t y) {
  if (Tools::in_bounds(x, y, 11, 8)){
    return matrix_right[y][x];
  }
  return -1;
}

void set_led(uint16_t x, uint16_t y, CRGB color) {
  int16_t pos = xy_to_pos(x, y);
  if(pos >= 0) {
    leds[pos] = color;    
  }
}

void setup() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
}


void update() {

  for (int y = 0; y < 8; y++) {
    for (int x = 0; x < 11; x++) {
    
      set_led(x, y, CRGB::Green);
    }    
  }
  
}

void draw() {
  FastLED.show();
}

void loop() {
  update();
  draw();
  delay(100);
}
