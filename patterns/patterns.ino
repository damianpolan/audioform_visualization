#include "FastLED.h"
#include "mixer.h"
#include "matrix.h"
#include "point.h"
#include "shapes.h"
#include "effects.h"
#include "tools.h"
#include "static_patterns.h"

#define NUM_LEDS 74
#define BRIGHTNESS 40

#define PIN_LEFT 6
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

int32_t FRAME_DELAY = 40;
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

	// double size view matrix
  view_matrix = new Matrix(view_width * 2, view_height * 2, Point(view_width, 0));
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

void copy_double_size_view_matrix() {
	Matrix* halved = view_matrix->half(Point(0, 0), Point(view_matrix->width, view_matrix->height));

	for (int32_t y = 0; y < halved->height; y++) {
		for (int32_t x = 0; x < halved->width; x++) {
			set_led(x, y, halved->get_absolute(x, y));
		}
	}

	delete halved;
}

void copy_view_matrix() {
	for (int32_t y = 0; y < view_matrix->height; y++) {
		for (int32_t x = 0; x < view_matrix->width; x++) {
			set_led(x, y, view_matrix->get_absolute(x, y));
		}
	}
}


//NOTES:
//

int y = 0;
int y2 = 14;
int32_t blur_dist[5] = { 10, 20, 50, 20, 10 };
void update() {
  // view_matrix->clear(CRGB(255, 0, 0));
	// Shapes::rectangle(view_matrix, Point(15, 2), Point(21, 6), CRGB::Blue);
  // Effects::translate(view_matrix, view_matrix, Point(0, y));

	// Shapes::circle(view_matrix, Point(18, 4), 4, CRGB::Blue);
  // Shapes::line(view_matrix, Point(30, 10), Point(48, 0), CRGB::Blue);
  // Shapes::line(view_matrix, Point(30, 12), Point(48, 2), CRGB::Red);
  // for (int32_t y = 0; y < view_width; y++) {
  //   for (int32_t x = -view_width; x < 0; x++) {
  //       view_matrix->set(x, y, CRGB::Blue);
  //   }
  // }


  Shapes::line(view_matrix, Point(-view_width, 0), Point(view_width, 0), CRGB::Blue);
  Shapes::line(view_matrix, Point(-view_width, 1), Point(view_width, 1), CRGB::Blue);
  Shapes::line(view_matrix, Point(-view_width, 2), Point(view_width, 2), CRGB::Blue);
  // Shapes::line(view_matrix, Point(0, 1), Point(-10, 10), CRGB::Blue);

  //
  // view_matrix->clear(CRGB(50, 0, 50));
  // Shapes::line(view_matrix, Point(0, 8), Point(48, y), CRGB::Blue);
  // Shapes::line(view_matrix, Point(0, 8), Point(48, y2), CRGB::Blue);
  // Effects::blur(view_matrix, view_matrix, blur_dist, 5);
  y += 2;
	y2 += 2;
	if (y > 22) {
		y = -6;
	}
  if (y2 > 22) {
    y2 = -6;
  }


}

void draw() {
	copy_double_size_view_matrix();
	FastLED.show();
}

void loop() {
	update();
	draw();
	delay(FRAME_DELAY);
}




