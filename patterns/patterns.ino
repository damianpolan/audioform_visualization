#include "FastLED.h"
#include "mixer.h"
#include "matrix.h"
#include "point.h"
#include "shapes.h"
#include "effects.h"
#include "tools.h"
#include "static_patterns.h"
#include <vector>

#define BRIGHTNESS 40

#define PIN_LEFT 6
#define PIN_RIGHT 8
#define PIN_BUTTON 11
#define PIN_INDICATOR 13

void set_led(int32_t x, int32_t y, CRGB color);
void display_double_size_view_matrix(Matrix* view_m);
void display_view_matrix(Matrix* view_m);
void set_view_matrix(bool use_double);
void button_interrupt();
void init_patterns();

// LED CONFIGURATION
#define NUM_LEDS 90
int32_t config_grid_right_width = 14;
int32_t config_grid_right_height = 8;
int32_t config_grid_num_leds = 90;
int32_t config_grid_gap = 1;
int32_t config_grid_right[8][14] = {
  { -1, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, -1, -1, -1 },
  { -1, 79, 78, 77, 76, 75, 74, 73, 72, 71, 70, 69, -1, -1 },
  { 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, -1, -1 },
  { 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, -1 },
  { 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43 },
  { -1, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, -1 },
  { -1, -1,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, -1, -1 },
  { -1, -1, -1,  7,  6,  5,  4,  3,  2,  1,  0, -1, -1, -1 } 
};




bool CAITLYNS_BRA = false;
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

	FastLED.addLeds<NEOPIXEL, PIN_LEFT>(leds_left, NUM_LEDS).setCorrection( CRGB(220, 230, 255) );
	FastLED.addLeds<NEOPIXEL, PIN_RIGHT>(leds_right, NUM_LEDS).setCorrection( CRGB(220, 230, 255)   );
	FastLED.setBrightness(BRIGHTNESS);

	screen_width = (2 * config_grid_right_width) + config_grid_gap;
	screen_height = config_grid_right_height;

	// double size view matrix
  view_matrix_double_res = new Matrix(screen_width * 2, screen_height * 2, Point(screen_width, 0));
  view_matrix_screen = new Matrix(screen_width, screen_height, Point(screen_width / 2, 0));

  set_view_matrix(false);
  init_patterns();
}


CRGB curr_transition_color;
CRGB curr_blueline_color = CRGB::Blue;
bool purple_haze(int32_t transition_counter, CRGB to_color) {
  set_view_matrix(false);
  FRAME_DELAY = 0;

  CRGB purple_b = CRGB(70, 0, 70);
  int32_t period = 20;

  if (transition_counter == -1) {
    view_matrix->clear(purple_b);
    curr_transition_color = purple_b;
  } else {
    if (transition_counter > 30)
      curr_blueline_color = Tools::step_color_towards(curr_blueline_color, to_color, 3);
    
    if (transition_counter % 1 == 0)
      curr_transition_color = Tools::step_color_towards(curr_transition_color, to_color, 1);
    view_matrix->clear(curr_transition_color);
    if (transition_counter > 80)
      return true;
  }


  int32_t half_counter = counter / 2;

  Shapes::line(view_matrix, Point(screen_width * 2, 8), Point(0, half_counter % period - 5), curr_blueline_color);
  Shapes::line(view_matrix, Point(screen_width * 2, 8 + 1), Point(0, half_counter % period - 5 + 1), curr_blueline_color);

  Shapes::line(view_matrix, Point(screen_width * 2, 8), Point(0, (half_counter - 8) % period - 5), curr_blueline_color);
  Shapes::line(view_matrix, Point(screen_width * 2, 8 + 1), Point(0, (half_counter - 8) % period - 5 + 1), curr_blueline_color);

  Shapes::line(view_matrix, Point(-screen_width * 2, 8), Point(0, half_counter % period - 5), curr_blueline_color);
  Shapes::line(view_matrix, Point(-screen_width * 2, 8 + 1), Point(0, half_counter % period - 5 + 1), curr_blueline_color);

  Shapes::line(view_matrix, Point(-screen_width * 2, 8), Point(0, (half_counter - 8) % period - 5), curr_blueline_color);
  Shapes::line(view_matrix, Point(-screen_width * 2, 8 + 1), Point(0, (half_counter - 8) % period - 8 + 1), curr_blueline_color);

  Effects::blur(view_matrix, view_matrix, blur_dist_5, 5);

  return false;
}


CRGB current_generation_color = CHSV(0, 255, 255);
bool random_spots(int32_t transition_counter, CRGB to_color) {
  set_view_matrix(false);
  FRAME_DELAY = 70;
  int32_t prob = 3;

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
    if(transition_counter > 40)
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

Matrix* line_states;
void init_patterns() {
  delete line_states;
  line_states = new Matrix(screen_width, screen_height);
  line_states->clear(CRGB(0, 0, 0));

  curr_blueline_color = CRGB::Blue;

  // int32_t left_middle_r = view_pad_left_inner - pad_size / 3 + 1;
  // int32_t left_middle_l = view_left + pad_size / 3 - 1;
  // int32_t right_middle_r = view_right - pad_size / 3 + 1;
  // int32_t right_middle_l = view_pad_right_inner + pad_size / 3 - 1;

  // vert_pos_left_l.left = view_left;
  // vert_pos_left_l.right = left_middle_l;

  // vert_pos_left_m.left = left_middle_l + 1;
  // vert_pos_left_m.right = left_middle_R - 1;

  // vert_pos_left_R.left = left_middle_R;
  // vert_pos_left_R.right = view_pad_left_inner;

  // vert_pos_right_l.left = view_pad_right_inner;
  // vert_pos_right_l.right = right_middle_l;

  // vert_pos_right_m.left = right_middle_l;
  // vert_pos_right_m.right = right_middle_r;

  // vert_pos_right_R.left = right_middle_r;
  // vert_pos_right_R.right = view_right;
}

bool flying_lines(int32_t transition_counter, CRGB to_color, CRGB background_color) {
  set_view_matrix(false);
  FRAME_DELAY = 50;
  int32_t prob = 7;

  if (transition_counter != -1) {
    prob = 3;

    if (transition_counter > 40) {
      return true;
    }
  }

  view_matrix->clear(background_color);
  for (int32_t y = 0; y < line_states->width; y++) {
    if(random(0, prob) <= 0) {
      line_states->set_absolute(0, y, CRGB(1, 0, 0));
      line_states->set_absolute(screen_width - 1, y, CRGB(1, 0, 0));
    }
  }

  for (int32_t y = 0; y < line_states->height; y++) {
    for (int32_t x = line_states->width / 2 + 1; x < line_states->width; x++) {
      if(line_states->get_absolute(x, y).r > 0) { // right pad
          view_matrix->set_absolute(x, y, CRGB(0, 255, 100));
          view_matrix->set_absolute(x + 1, y, CRGB(0, 255, 50));
          view_matrix->set_absolute(x + 2, y, CRGB(0, 255, 0));

          line_states->set_absolute(x, y, CRGB(0, 0, 0));

          if (line_states->get_absolute(x - 1, y).g != 2) {
            line_states->set_absolute(x - 1, y, CRGB(1, 0, 0));
          }
          if (transition_counter != -1) {
            if (x == line_states->width / 2 + 1 || line_states->get_absolute(x - 1, y).g == 2) {
              line_states->set_absolute(x, y, CRGB(0, 2, 0));
            }
          }
      }
      if (line_states->get_absolute(x, y).g == 2) {
        view_matrix->set_absolute(x, y, to_color);
      }
    }
  }

  for (int32_t y = 0; y < line_states->height; y++) {
    for (int32_t x = line_states->width / 2 - 1; x >= 0; x--) {
      if(line_states->get_absolute(x, y).r > 0) { // left pad
          view_matrix->set_absolute(x, y, CRGB(0, 255, 100));
          view_matrix->set_absolute(x - 1, y, CRGB(0, 255, 50));
          view_matrix->set_absolute(x - 2, y, CRGB(0, 255, 0));

          line_states->set_absolute(x, y, CRGB(0, 0, 0));

          if (line_states->get_absolute(x + 1, y).g != 2) {
            line_states->set_absolute(x + 1, y, CRGB(1, 0, 0));
          }
          if (transition_counter != -1) {
            if (x == line_states->width / 2 - 1 || line_states->get_absolute(x + 1, y).g == 2) {
              line_states->set_absolute(x, y, CRGB(0, 2, 0));
            }
          }
      }
      if (line_states->get_absolute(x, y).g == 2) {
        view_matrix->set_absolute(x, y, to_color);
      }
    }
  }


  return false;
}


typedef struct {
  int32_t left = 0;
  int32_t right = 0;
  int32_t top = 0;
  int32_t state = 0;
  CRGB color = CRGB(0, 0, 0);
} CBox;
CBox vert_pos_left_l;
CBox vert_pos_left_m;
CBox vert_pos_left_R;
CBox vert_pos_right_l;
CBox vert_pos_right_m;
CBox vert_pos_right_R;

bool canada_flag(int32_t transition_counter, CRGB to_color) {
  set_view_matrix(false);
  int32_t prob = 10;

  view_matrix->clear(CRGB::White);

  int32_t pad_size = view_pad_left_inner - view_left;

  int32_t left_middle_r = view_pad_left_inner - pad_size / 3 + 1;
  int32_t left_middle_l = view_left + pad_size / 3 - 1;

  int32_t right_middle_r = view_right - pad_size / 3 + 1;
  int32_t right_middle_l = view_pad_right_inner + pad_size / 3 - 1;

  Shapes::fill_rectangle(view_matrix, Point(view_left, view_top), Point(left_middle_l, view_bottom), CRGB::Red);
  Shapes::fill_rectangle(view_matrix, Point(left_middle_r, view_top), Point(view_pad_left_inner, view_bottom), CRGB::Red);

  Shapes::fill_rectangle(view_matrix, Point(view_pad_left_inner, view_top), Point(right_middle_l, view_bottom), CRGB::Red);
  Shapes::fill_rectangle(view_matrix, Point(right_middle_r, view_top), Point(view_right, view_bottom), CRGB::Red);

  if (random(prob) <= 1) {

  }

  return false;
}



bool gradient_color_change(int32_t transition_counter, CRGB to_color) {
  set_view_matrix(false);

  for (int32_t y = 0; y < view_matrix->height; y++) {
    for (int32_t x = 0; x < view_matrix->width; x++) {



      CRGB color_n;

      if (counter < 50) {
        color_n = CHSV(((counter * 5) % 255) + x + y * 5, counter * 4, counter * 4);
      } else
        color_n = CHSV(((counter * 5) % 255) + x + y * 5, 200, 200);


      if (transition_counter != -1) {
        color_n = Tools::step_color_towards(color_n, to_color, transition_counter);
      }

      view_matrix->set_absolute(x, y, color_n);
      
    }
  }

  if (transition_counter > 50)
  {
    return true;
  }

  return false;
}



bool tan_function(int32_t transition_counter, CRGB to_color) {
  for (int32_t y = 0; y < view_matrix->height; y++) {
    for (int32_t x = 0; x < view_matrix->width; x++) {
      CRGB color_n = CHSV( sin( ((float) counter) / 10) * 100, 200, 200);
    }
  }

  return false;
}






int32_t current_pattern = 0;
int32_t current_pattern_count = 5;
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
      is_done = tan_function(transition_counter, CRGB(10, 10, 10));
      break;
    case 1:
      is_done = gradient_color_change(transition_counter, CRGB(70, 0, 70));
      break;
    case 2:
      is_done = purple_haze(transition_counter, CRGB(60, 60, 60));
      break;
    case 3:
      is_done = flying_lines(transition_counter, CRGB(0, 0, 0), CRGB(60, 60, 60));
      break;
    case 4:
      is_done = random_spots(transition_counter, CRGB::White);
      break;
  }

  if (is_done) {
    counter = 0;
    transition_counter = -1;
    current_pattern++;
    init_patterns();
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
void clear_now() {
  FastLED.clear();
  FastLED.show();
}

int32_t Frame_Delay_timer = 50;

bool strobe = false;
int32_t strobe_millis_on = 5;
int32_t strobe_millis_off = 50;
unsigned long time_update_start;
unsigned long time_draw_start;
void loop() {
  time_update_start = millis();
	update();
  
  if (strobe) while(millis() - time_update_start < strobe_millis_off); 
  else while(millis() - time_update_start < Frame_Delay_timer); 
  
  time_draw_start = millis();
  draw();  

  if (strobe) {
    while(millis() - time_draw_start < strobe_millis_on);
    clear_now(); 
  }
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

CRGB caitlyn_correct(int32_t x, int32_t y, CRGB color) {
  if (CAITLYNS_BRA
    && ( (x > screen_width / 2 && y == 4) 
    || (x < screen_width / 2 && y == 0))) {

    int32_t red = color.r * 1;
    int32_t green = color.g * 0.6f;
    int32_t blue = color.b * 0.6f;

    red = std::max((int) red, 0);
    green = std::max((int) green, 0);
    blue = std::max((int) blue, 0);

    return CRGB(red, green, blue);
  }
  return color;
}

void set_led(int32_t x, int32_t y, CRGB color) {
  //zero point (x = 0) is in the middle of the bra gap 
  if (Tools::in_bounds(x, y, screen_width, screen_height)) {
    if (x < config_grid_right_width) {
      int32_t rel_x = config_grid_right_width - x - 1;
      int32_t pos = config_grid_right[y][rel_x];
      color = caitlyn_correct(x, y, color);
      if (pos > -1) leds_left[pos] = color;
    }
    else if (x >= config_grid_right_width + config_grid_gap) {
      int32_t rel_x = x - (config_grid_right_width + config_grid_gap);
      int32_t pos = config_grid_right[y][rel_x];
      color = caitlyn_correct(x, y, color);
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
