#include "FastLED.h"
#include "mixer.h"
#include "matrix.h"
#include "point.h"
#include "shapes.h"
#include "tools.h"

#define NUM_LEDS 74
#define BRIGHTNESS 10

#define PIN_LEFT 9
#define PIN_RIGHT 8

// LED CONFIGURATION
int32_t config_grid_right_width = 11;
int32_t config_grid_right_height = 8;
int32_t config_grid_num_leds = 74;
int32_t config_grid_gap = 2;
int32_t config_grid_right[8][11] = {
	{ -1, -1, 67, 68, 69, 70, 71, 72, 73, -1, -1 },
	{ -1, 66, 65, 64, 63, 62, 61, 60, 59, 58, -1 },
	{ 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, -1 },
	{ 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37 },
	{ 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36 },
	{ -1, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16 },
	{ -1, -1, 7, 8, 9, 10, 11, 12, 13, 14, 15 },
	{ -1, -1, -1, 6, 5, 4, 3, 2, 1, 0, -1 }
};


int32_t view_width;
int32_t view_height;
CRGB leds_left[NUM_LEDS];
CRGB leds_right[NUM_LEDS];
Matrix* view_matrix;

void setup() {
	Serial.begin(9600);
	Serial.print("Start\n\r");
	FastLED.addLeds<NEOPIXEL, PIN_LEFT>(leds_left, NUM_LEDS);
	FastLED.addLeds<NEOPIXEL, PIN_RIGHT>(leds_right, NUM_LEDS);
	FastLED.setBrightness(BRIGHTNESS);

	view_width = (2 * config_grid_right_width) + config_grid_gap;
	view_height = config_grid_right_height;
	view_matrix = new Matrix(view_width, view_height);
}


void set_led(int32_t x, int32_t y, CRGB color) {

  if (Tools::in_bounds(x, y, view_width, view_height)) {
    if (x < config_grid_right_width) {
      int32_t rel_x = config_grid_right_width - x;
      int32_t pos = config_grid_right[y][rel_x];
      if (pos > -1) leds_left[pos] = color;
    }
    else if (x >= config_grid_right_width + config_grid_gap) {
      int32_t rel_x = x - (config_grid_right_width + config_grid_gap);
      int32_t pos = config_grid_right[y][rel_x];
      if (pos > -1) leds_right[pos] = color;
    }
  } 
}

void copy_view_matrix() {
	for (int32_t y = 0; y < view_matrix->height; y++) {
		for (int32_t x = 0; x < view_matrix->width; x++) {
			Serial.print("set_led\n\r");
			set_led(x, y, view_matrix->get(x, y));
		}
	}
}


void update() {
	Serial.print("Update\n\r");
	view_matrix->set(16, 5, CRGB::Azure);
	Serial.print("copy_view_matrix\n\r");
	copy_view_matrix();
}

void draw() {
	FastLED.show();
}

void loop() {
	update();
	draw();
	delay(500);
}




