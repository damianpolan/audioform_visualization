#include "tools.h"

bool Tools::in_bounds(int32_t x, int32_t y, int32_t width, int32_t height) {
	return x >= 0 && x < width && y >= 0 && y < height;
}

bool Tools::in_bounds(int32_t x, int32_t y, Point p0, Point p1) {
	return x >= p0.x && x < p1.x && y >= p0.y && y < p1.y;
}

CRGB Tools::step_color_towards(CRGB original, CRGB target, int32_t amount) {
	int32_t red = original.r;
	int32_t green = original.g;
	int32_t blue = original.b;

	if (original.r < target.r){
		red += amount;
		if (red > target.r)
			red = target.r;
	} else {
		red -= amount;
		if (red < target.r)
			red = target.r;
	}

	if (original.g < target.g){
		green += amount;
		if (green > target.g)
			green = target.g;
	} else {
		green -= amount;
		if (green < target.g)
			green = target.g;
	}

	if (original.b < target.b){
		blue += amount;
		if (blue > target.b)
			blue = target.b;
	} else {
		blue -= amount;
		if (blue < target.b)
			blue = target.b; 
	}

	red = max(min(255, red), 0);
	green = max(min(255, green), 0);
	blue = max(min(255, blue), 0);

	return CRGB(red, green, blue);
}
