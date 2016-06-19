#include "effects.h"


void Effects::translate(Matrix* target, Matrix* sourceA, Point amount) {
	Matrix* placeholder = target;
	if (target == sourceA) {
		placeholder = new Matrix(sourceA->width, sourceA->height);
	}

	for (int32_t y = 0; y < sourceA->height; y++) {
		for (int32_t x = 0; x < sourceA->width; x++) {
			placeholder->set_absolute(x, y, sourceA->get_absolute(x - amount.x, y - amount.y));
		}
	}

	if (target == sourceA) {
		for (int32_t y = 0; y < placeholder->height; y++) {
			for (int32_t x = 0; x < placeholder->width; x++) {
				target->set_absolute(x, y, placeholder->get_absolute(x, y));
			}
		}
		delete placeholder;
	}
}

void Effects::rotate(Matrix* target, Matrix* sourceA, int32_t center_x, int32_t center_y, int32_t degrees) {

}

void Effects::scale(Matrix* target, Matrix* sourceA, int32_t center_x, int32_t center_y, int32_t factor) {

}


// Example call: Effects.blue(target, sourceA, [10, 80, 10], 3)
// dist[] must sum to 100.
// FROM: http://stackoverflow.com/questions/1696113/how-do-i-gaussian-blur-an-image-without-using-any-in-built-gaussian-functions
void Effects::blur(Matrix* target, Matrix* sourceA, int32_t dist[], int32_t dist_len) {
	Matrix* placeholder = target;
	if (target == sourceA) {
		placeholder = new Matrix(sourceA->width, sourceA->height);
	}

	// define box filter matrix (degree 0 = no blur)
	int32_t** box_filter;
	const int32_t divider = 10000; //division required to take the final number back to normal scale
	
	box_filter = new int32_t*[dist_len];
	for (int32_t y = 0; y < dist_len; y++) {
		box_filter[y] = new int32_t[dist_len];
		for (int32_t x = 0; x < dist_len; x++) {
			box_filter[y][x] = dist[x] * dist[y];
		}
	}

	for (int32_t y = 0; y < sourceA->height; y++) {
		for (int32_t x = 0; x < sourceA->width; x++) {
			uint32_t red = 0;
			uint32_t green = 0;
			uint32_t blue = 0;

			for (int32_t bf_y = 0; bf_y < dist_len; bf_y++) {
				for (int32_t bf_x = 0; bf_x < dist_len; bf_x++) {
					int32_t real_y = y + (bf_y - dist_len / 2);
					int32_t real_x = x + (bf_x - dist_len / 2);

					if (Tools::in_bounds(real_x, real_y, sourceA->width, sourceA->height)) {
						red += sourceA->get_absolute(real_x, real_y).r * box_filter[bf_y][bf_x];
						green += sourceA->get_absolute(real_x, real_y).g * box_filter[bf_y][bf_x];
						blue += sourceA->get_absolute(real_x, real_y).b * box_filter[bf_y][bf_x];
					}
				}
			}

			red /= divider;
			green /= divider;
			blue /= divider;

			placeholder->set_absolute(x, y, CRGB(red, green, blue));
		}
	}


	for (int32_t y = 0; y < dist_len; y++) {
		delete box_filter[y];
	}
	delete box_filter;
	if (target == sourceA) {
		for (int32_t y = 0; y < placeholder->height; y++) {
			for (int32_t x = 0; x < placeholder->width; x++) {
				target->set_absolute(x, y, placeholder->get_absolute(x, y));
			}
		}
		delete placeholder;
	}
}