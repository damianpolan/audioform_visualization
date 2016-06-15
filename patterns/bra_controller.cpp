#include "bra_controller.h"

#define PIN_LEFT 9
#define PIN_RIGHT 8

int32_t configuration_0_right_width = 11;
int32_t configuration_0_right_height = 8;
int32_t configuration_0_num_leds = 74;
int32_t configuration_0_gap = 2;
int32_t configuration_0_right[8][11] = {
  {-1, -1, 67, 68, 69, 70, 71, 72, 73, -1, -1},
  {-1, 66, 65, 64, 63, 62, 61, 60, 59, 58, -1},  
  {48, 49, 50, 51, 52, 53, 54, 55, 56, 57, -1}, 
  {47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37}, 
  {26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36}, 
  {-1, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16}, 
  {-1, -1, 7, 8, 9, 10, 11, 12, 13, 14, 15}, 
  {-1, -1, -1, 6, 5, 4, 3, 2, 1, 0, -1}
};


void Controller::initialize_matrix_right(int32_t** configuration, int32_t width, int32_t height) {
  this->matrix_right = new int32_t*[height];
  for (int32_t y = 0; y < height; y++) {
    this->matrix_right[y] = new int32_t[width];
    for (int32_t x = 0; x < width; x++) {
      //TODO: MIGHT NOT WORK, CHECK http://www.geeksforgeeks.org/pass-2d-array-parameter-c/
      this->matrix_right[y][x] = configuration[y][x];
    }
  }
}

Controller::Controller(int32_t bra_type, int8_t brightness) {
  if (bra_type == 0) {
    this->initialize_matrix_right((int32_t**) configuration_0_right, configuration_0_right_width, configuration_0_right_height);
    // this->width = configuration_0_right_width; // * 2 + gap
    // this->height = configuration_0_right_height;
    // this->gap = configuration_0_gap;

    // this->leds_length = configuration_0_num_leds;
  }


  // this->leds_left = new CRGB[this->leds_length];
  // this->leds_right = new CRGB[this->leds_length];
  // this->leds_right[0] = CRGB::Blue;
  // FastLED.addLeds<NEOPIXEL, PIN_LEFT>(this->leds_left, this->leds_length);
  // FastLED.addLeds<NEOPIXEL, PIN_RIGHT>(this->leds_right, this->leds_length);
  // FastLED.setBrightness(brightness);
}


void Controller::set_led(int32_t x, int32_t y, CRGB color) {
  if (Tools::in_bounds(x, y, 11, 8)){
    this->leds_right[y][x] = color;
  }
}


void Controller::draw() {
  FastLED.show();
}