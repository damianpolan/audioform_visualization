#include "effects.h"


void Effects::translate(Matrix* target, Matrix* sourceA, int16_t amount_x, int16_t amount_y) {
	Matrix* placeholder = target;
	if (target == sourceA) {
		placeholder = new Matrix(sourceA->width, sourceA->height);
	}

	for (uint16_t y = 0; y < sourceA->height; y++) {
		for (uint16_t x = 0; x < sourceA->width; x++) {
			placeholder->set(x, y, sourceA->get(x - amount_x, y - amount_y));
		}
	}

	if (target == sourceA) {
		for (uint16_t y = 0; y < placeholder->height; y++) {
			for (uint16_t x = 0; x < placeholder->width; x++) {
				target->set(x, y, placeholder->get(x, y));
			}
		}
	}
}

void Effects::rotate(Matrix* target, Matrix* sourceA, int16_t center_x, int16_t center_y, int16_t degrees) {

}

void Effects::scale(Matrix* target, Matrix* sourceA, int16_t center_x, int16_t center_y, int16_t factor) {

}


// Example call: Effects.blue(target, sourceA, [10, 80, 10], 3)
// dist[] must sum to 100.
// FROM: http://stackoverflow.com/questions/1696113/how-do-i-gaussian-blur-an-image-without-using-any-in-built-gaussian-functions
void Effects::blur(Matrix* target, Matrix* sourceA, int16_t dist[], int16_t dist_len) {
	Matrix* placeholder = target;
	if (target == sourceA) {
		placeholder = new Matrix(sourceA->width, sourceA->height);
	}

	// define box filter matrix (degree 0 = no blur)
	int16_t** box_filter;
	const int16_t divider = 10000; //division required to take the final number back to normal scale

	box_filter = new int16_t*[dist_len];
	for (uint16_t y = 0; y < dist_len; y++) {
		box_filter[y] = new int16_t[dist_len];
		for (uint16_t x = 0; x < dist_len; x++) {
			box_filter[y][x] = dist[x] * dist[y];
		}
	}

	for (uint16_t y = 0; y < sourceA->height; y++) {
		for (uint16_t x = 0; x < sourceA->width; x++) {
			uint32_t red = 0;
			uint32_t green = 0;
			uint32_t blue = 0;

			for (uint16_t bf_y = 0; bf_y < dist_len; bf_y++) {
				for (uint16_t bf_x = 0; bf_x < dist_len; bf_x++) {
					uint16_t real_y = y + (bf_y - 1);
					uint16_t real_x = x + (bf_x - 1);

					if (Tools::in_bounds(real_x, real_y, sourceA->width, sourceA->height)) {
						red += sourceA->get(real_x, real_y).r * box_filter[bf_y][bf_x];
						green += sourceA->get(real_x, real_y).g * box_filter[bf_y][bf_x];
						blue += sourceA->get(real_x, real_y).b * box_filter[bf_y][bf_x];
					}
				}
			}

			red /= divider;
			green /= divider;
			blue /= divider;

			placeholder->set(x, y, CRGB(red, green, blue));
		}
	}


	if (target == sourceA) {
		for (uint16_t y = 0; y < placeholder->height; y++) {
			for (uint16_t x = 0; x < placeholder->width; x++) {
				target->set(x, y, placeholder->get(x, y));
			}
		}
	}
}