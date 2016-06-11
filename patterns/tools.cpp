#include "tools.h"

bool Tools::in_bounds(int16_t x, int16_t y, uint16_t width, uint16_t height) {
	return x >= 0 && x < width && y >= 0 && y < height;
}
