#ifndef BRA_CONTROLLER
#define BRA_CONTROLLER


#include "FastLED.h"
#include "matrix.h"
#include "tools.h"

class Controller {
  private:
    int32_t** matrix_right; //right breast (relative to watcher not wearer)
    int32_t gap;
    CRGB* leds_left;
    CRGB* leds_right;
    CRGB leds_length;
    void initialize_matrix_right(int32_t** configuration, int32_t width, int32_t height);
  public:
    int32_t width;
    int32_t height;
    int32_t bra_type;
    Controller(int32_t bra_type, int8_t brightness);
  	void set_led(int32_t x, int32_t y, CRGB color);
    void draw();
};

#endif