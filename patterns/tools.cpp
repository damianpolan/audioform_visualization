#include "tools.h"

bool Tools::in_bounds(int32_t x, int32_t y, int32_t width, int32_t height) {
	return x >= 0 && x < width && y >= 0 && y < height;
}

bool Tools::in_bounds(int32_t x, int32_t y, Point p0, Point p1) {
	return x >= p0.x && x < p1.x && y >= p0.y && y < p1.y;
}