#include "tools.h"

bool Tools::in_bounds(int32_t x, int32_t y, int32_t width, int32_t height) {
	return x >= 0 && x < width && y >= 0 && y < height;
}
