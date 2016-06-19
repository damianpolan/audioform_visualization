#include "FastLED.h"
#include "mixer.h"
#include "matrix.h"
#include "point.h"
#include "shapes.h"
#include "effects.h"
#include "tools.h"
#include "static_patterns.h"

#define NUM_LEDS 74
#define BRIGHTNESS 100

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



bool on = false;
void update() {

  // view_matrix->clear(CRGB(0, 100, 0));
  // Shapes::rectangle(view_matrix, Point(view_left, view_top), Point(view_right, view_bottom), CRGB::Green);
  // Shapes::rectangle(view_matrix, Point(view_pad_left_inner, view_top), Point(view_pad_right_inner, view_bottom), CRGB::Green);


  // FIFTY FIFTY
  // StaticPatterns::fifty_fifty(view_matrix, CRGB::White, CRGB::Red);
 //  // StaticPatterns::fifty_fifty(view_matrix, CRGB(25, 158, 216), CRGB(7, 113, 184));


  // #############################################################
  // BLURRED Horizontal LINE
  // view_matrix->clear(CRGB(0, 100, 100));
  // Shapes::line(view_matrix, Point(view_left, view_bottom / 2 + 1), Point(view_right, view_bottom / 2 + 1), CRGB::White);
  // Shapes::line(view_matrix, Point(view_left, view_bottom / 2), Point(view_right + 1, view_bottom / 2), CRGB::White);
  // Effects::blur(view_matrix, view_matrix, blur_dist_5, 5);

  
  // #############################################################
  // BLURRED Vertical lines
  // view_matrix->clear(CRGB(0, 100, 100));
  // Shapes::line(view_matrix, Point(view_left + 4, view_top), Point(view_left + 4, view_bottom), CRGB::White);
  // Shapes::line(view_matrix, Point(view_left + 5, view_top), Point(view_left + 5, view_bottom), CRGB::White);
  // Shapes::line(view_matrix, Point(view_right - 4, view_top), Point(view_right - 4, view_bottom), CRGB::White);
  // Shapes::line(view_matrix, Point(view_right - 5, view_top), Point(view_right - 5, view_bottom), CRGB::White);
  // Effects::blur(view_matrix, view_matrix, blur_dist_5, 5);

  // #############################################################
  // CROSS lines
  view_matrix->clear(CRGB(0, 100, 100));
  Shapes::line(view_matrix, Point(view_left + 4, view_top), Point(view_left + 4, view_bottom), CRGB::White);
  Shapes::line(view_matrix, Point(view_left + 5, view_top), Point(view_left + 5, view_bottom), CRGB::White);
  Shapes::line(view_matrix, Point(view_right - 4, view_top), Point(view_right - 4, view_bottom), CRGB::White);
  Shapes::line(view_matrix, Point(view_right - 5, view_top), Point(view_right - 5, view_bottom), CRGB::White);
  Shapes::line(view_matrix, Point(view_left, view_bottom / 2 + 1), Point(view_right, view_bottom / 2 + 1), CRGB::White);
  Shapes::line(view_matrix, Point(view_left, view_bottom / 2), Point(view_right + 1, view_bottom / 2), CRGB::White);
  // Effects::blur(view_matrix, view_matrix, blur_dist_5, 5);



  //light blue
  // CRGB(25, 158, 216), CRGB(7, 113, 184)

  // if (on) {
  //   view_matrix->clear(CRGB(100, 0, 0));
  //   Shapes::rectangle(view_matrix, Point(-5, 2), Point(5, 5), CRGB::Blue);    
  // }

  // set_view_matrix(true);
  // FRAME_DELAY = 0;
  // view_matrix->clear(CRGB(50, 0, 50));
  // Shapes::line(view_matrix, Point(screen_width * 2, 8), Point(0, counter % 50 - 8), CRGB::Blue);
  // Shapes::line(view_matrix, Point(screen_width * 2, 8 + 1), Point(0, counter % 50 - 8 + 1), CRGB::Blue);

  // Shapes::line(view_matrix, Point(screen_width * 2, 8), Point(0, counter % 50 - 16), CRGB::Blue);
  // Shapes::line(view_matrix, Point(screen_width * 2, 8 + 1), Point(0, counter % 50 - 16 + 1), CRGB::Blue);

  // Shapes::line(view_matrix, Point(-screen_width * 2, 8), Point(0, counter % 50 - 8), CRGB::Blue);
  // Shapes::line(view_matrix, Point(-screen_width * 2, 8 + 1), Point(0, counter % 50 - 8 + 1), CRGB::Blue);

  // Shapes::line(view_matrix, Point(-screen_width * 2, 8), Point(0, counter % 50 - 16), CRGB::Blue);
  // Shapes::line(view_matrix, Point(-screen_width * 2, 8 + 1), Point(0, counter % 50 - 16 + 1), CRGB::Blue);

  // Effects::blur(view_matrix, view_matrix, blur_dist_5, 5);

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
