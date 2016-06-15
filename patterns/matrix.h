#ifndef MATRIX
#define MATRIX

#include "FastLED.h"
#include "tools.h"
#include <vector>

class Matrix {
	public:
		std::vector<std::vector<CRGB>> matrix2D;
		int32_t width;
		int32_t height;
		Matrix(int32_t width, int32_t height);
		CRGB get(int32_t x, int32_t y);
		void set(int32_t x, int32_t y, CRGB color);
		Matrix* copy();
};

#endif
