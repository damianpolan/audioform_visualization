#include "FastLED.h"
#include "mixer.h"
#include "matrix.h"
#include "point.h"
#include "shapes.h"
#include "effects.h"
#include "tools.h"
#include "static_patterns.h"
#include <vector>

#define NUM_LEDS 74
#define BRIGHTNESS 20

#define PIN_LEFT 6
#define PIN_RIGHT 8
#define PIN_BUTTON 11
#define PIN_INDICATOR 13

void set_led(int32_t x, int32_t y, CRGB color);
void display_double_size_view_matrix(Matrix* view_m);
void display_view_matrix(Matrix* view_m);
void set_view_matrix(bool use_double);
void button_interrupt();

// LED CONFIGURATION
int32_t config_grid_right_width = 11;
int32_t config_grid_right_height = 8;
int32_t config_grid_num_leds = 74;
int32_t config_grid_gap = 1;// must be odd
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
int32_t screen_width; // real dimensions of the LED screen.
int32_t screen_height;
CRGB leds_left[NUM_LEDS];
CRGB leds_right[NUM_LEDS];
Matrix* view_matrix_screen;
Matrix* view_matrix_double_res;
bool use_double_view_matrix;
Matrix* view_matrix; //the currently assigned view_matrix;

//helper variables
int32_t view_center_x = 0;
int32_t view_center_y = 0;
int32_t view_left;
int32_t view_right;
int32_t view_top;
int32_t view_bottom;
int32_t view_gap_size;
int32_t view_pad_left_inner;
int32_t view_pad_right_inner;
int32_t view_width;
int32_t view_height;
int32_t counter = 0;
int32_t blur_dist_5 [5] = { 10, 20, 40, 20, 10 };
int32_t blur_dist_3_hard [3] = { 10, 80, 10 };
int32_t blur_dist_3_soft [3] = { 20, 60, 20 };

bool on = false;

void setup() {
	Serial.begin(9600);

	pinMode(PIN_INDICATOR, OUTPUT);
  digitalWrite(PIN_INDICATOR, HIGH);

  // pinMode(PIN_BUTTON, INPUT);
  // attachInterrupt(digitalPinToInterrupt(PIN_BUTTON), button_interrupt, FALLING );

	FastLED.addLeds<NEOPIXEL, PIN_LEFT>(leds_left, NUM_LEDS).setCorrection( TypicalLEDStrip );
	FastLED.addLeds<NEOPIXEL, PIN_RIGHT>(leds_right, NUM_LEDS).setCorrection( TypicalLEDStrip );
	FastLED.setBrightness(BRIGHTNESS);

	screen_width = (2 * config_grid_right_width) + config_grid_gap;
	screen_height = config_grid_right_height;

	// double size view matrix
  view_matrix_double_res = new Matrix(screen_width * 2, screen_height * 2, Point(screen_width, 0));
  view_matrix_screen = new Matrix(screen_width, screen_height, Point(screen_width / 2, 0));

  set_view_matrix(false);

}


CRGB curr_transition_color;
bool purple_haze(int32_t transition_counter, CRGB to_color) {
  set_view_matrix(true);
  FRAME_DELAY = 0;

  CRGB purple_b = CRGB(70, 0, 70);
  int32_t period = 35;

  if (transition_counter == -1) {
    view_matrix->clear(purple_b);
    curr_transition_color = purple_b;
  } else {
    period += 50;
    curr_transition_color = Tools::step_color_towards(curr_transition_color, to_color, 4);
    view_matrix->clear(curr_transition_color);
    if (transition_counter > 50)
      return true;
  }

  Shapes::line(view_matrix, Point(screen_width * 2, 8), Point(0, counter % period - 8), CRGB::Blue);
  Shapes::line(view_matrix, Point(screen_width * 2, 8 + 1), Point(0, counter % period - 8 + 1), CRGB::Blue);

  Shapes::line(view_matrix, Point(screen_width * 2, 8), Point(0, (counter - 16) % period - 8), CRGB::Blue);
  Shapes::line(view_matrix, Point(screen_width * 2, 8 + 1), Point(0, (counter - 16) % period - 8 + 1), CRGB::Blue);

  Shapes::line(view_matrix, Point(-screen_width * 2, 8), Point(0, counter % period - 8), CRGB::Blue);
  Shapes::line(view_matrix, Point(-screen_width * 2, 8 + 1), Point(0, counter % period - 8 + 1), CRGB::Blue);

  Shapes::line(view_matrix, Point(-screen_width * 2, 8), Point(0, (counter - 16) % period - 8), CRGB::Blue);
  Shapes::line(view_matrix, Point(-screen_width * 2, 8 + 1), Point(0, (counter - 16) % period - 8 + 1), CRGB::Blue);

  Effects::blur(view_matrix, view_matrix, blur_dist_5, 5);

  return false;
}


CRGB current_generation_color = CHSV(0, 255, 255);
bool random_spots(int32_t transition_counter, CRGB to_color) {
  set_view_matrix(false);
  FRAME_DELAY = 70;
  int32_t prob = 4;

  if (transition_counter == -1) {
    view_matrix->clear(CRGB::Black);

    if (counter % 4 == 0) {
      CHSV c_step;
      c_step = rgb2hsv_approximate(current_generation_color);
      c_step.h += 2;
      hsv2rgb_rainbow(c_step, current_generation_color); 
    }
  } else {
    prob *= 2;
    current_generation_color = Tools::step_color_towards(current_generation_color, to_color, 8);
    if(transition_counter > 50)
      return true;
  }

  for (int32_t y = 0; y < view_matrix->height; y++) {
    for (int32_t x = 0; x < view_matrix->width; x++) {
      if(random(0, prob) <= 0) {
        view_matrix->set_absolute(x, y, current_generation_color);
      }
    }
  }

  return false;
}

bool canada_flag(int32_t transition_counter, CRGB to_color) {
  return false;
}

int32_t current_pattern = 0;
int32_t current_pattern_count = 2;
int32_t switch_period = 100;
int32_t max_trans_period = 100;
void update() {
  int32_t transition_counter = -1;
  int32_t cycling_counter = counter % (switch_period + max_trans_period);
  if (cycling_counter > switch_period) {
    transition_counter = counter - switch_period;
  }

  bool is_done = false;
  switch (current_pattern)
  {
     case 0:
        is_done = random_spots(transition_counter, CRGB(70, 0, 70));
        break;
     case 1:
        is_done = purple_haze(transition_counter, CRGB::Black);
        break;
  }

  if (is_done) {
    counter = 0;
    transition_counter = -1;
    current_pattern++;
    if (current_pattern >= current_pattern_count)
      current_pattern = 0;
  }
}


void draw() {
  if (use_double_view_matrix) 
    display_double_size_view_matrix(view_matrix_double_res);
  else 
    display_view_matrix(view_matrix_screen);
  
	FastLED.show();
  counter++;
}

void loop() {
	update();
	draw();
	delay(FRAME_DELAY);
}


void button_interrupt(){
  on = true;
}

void set_view_matrix(bool use_double) {
  use_double_view_matrix = use_double;
  if (use_double_view_matrix) {
    view_matrix = view_matrix_double_res;
    view_left = -screen_width;
    view_right = screen_width - 1;
    view_top = 0;
    view_bottom = screen_height * 2 - 1;
    view_gap_size = config_grid_gap * 2;
    view_pad_left_inner = -config_grid_gap * 2;
    view_pad_right_inner = config_grid_gap * 2;
    view_width = view_matrix_double_res->width;
    view_height = view_matrix_double_res->height;
  } else {
    view_matrix = view_matrix_screen;
    view_left = -screen_width / 2;
    view_right = screen_width / 2 ;
    view_bottom = screen_height - 1;
    view_gap_size = config_grid_gap;
    view_pad_left_inner = -config_grid_gap;
    view_pad_right_inner = config_grid_gap;
    view_width = view_matrix_screen->width;
    view_height = view_matrix_screen->height;
  }
}

void set_led(int32_t x, int32_t y, CRGB color) {
  //zero point (x = 0) is in the middle of the bra gap 
  if (Tools::in_bounds(x, y, screen_width, screen_height)) {
    if (x < config_grid_right_width) {
      int32_t rel_x = config_grid_right_width - x - 1;
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

void display_double_size_view_matrix(Matrix* view_m) {
  Matrix* halved = view_m->half(Point(0, 0), Point(view_m->width, view_m->height));
  for (int32_t y = 0; y < halved->height; y++) {
    for (int32_t x = 0; x < halved->width; x++) {
      set_led(x, y, halved->get_absolute(x, y));
    }
  }
  delete halved;
}

void display_view_matrix(Matrix* view_m) {
  for (int32_t y = 0; y < view_m->height; y++) {
    for (int32_t x = 0; x < view_m->width; x++) {
      set_led(x, y, view_m->get_absolute(x, y));
    }
  }
}
