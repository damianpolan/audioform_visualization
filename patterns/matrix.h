#ifndef MATRIX
#define MATRIX

#include "FastLED.h"
#include "point.h"
#include "tools.h"
#include <vector>

class Matrix {
	public:
		std::vector<std::vector<CRGB>> matrix2D;
		int32_t width;
		int32_t height;    
		int32_t pos_offset_x;
		int32_t pos_offset_y;
		Matrix(int32_t width, int32_t height);
		Matrix(int32_t width, int32_t height, Point pos_offset);
		CRGB get_absolute(int32_t x, int32_t y);
		CRGB get(int32_t x, int32_t y);
		void set(int32_t x, int32_t y, CRGB color);
		void set_absolute(int32_t x, int32_t y, CRGB color);
		void clear(CRGB color);
		Matrix* copy();
		Matrix* half(Point subbox_p0, Point subbox_p1); 
		// Creates a lower resolution matrix. Averaging the pixels down.
		// the subbox is the rectangle to reduce from. 
};

#endif
