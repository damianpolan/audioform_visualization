#ifndef MATRIX
#define MATRIX

#include "FastLED.h"

class Matrix {
	CRGB** matrix2D;
	public:
		uint16_t width;
		uint16_t height;
		Matrix(uint16_t width, uint16_t height);
		CRGB get(uint16_t x, uint16_t y);
		CRGB set(uint16_t x, uint16_t y, CRGB color);
};

#endif