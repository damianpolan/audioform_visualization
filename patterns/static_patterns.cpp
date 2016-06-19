#include "static_patterns.h"

void StaticPatterns::vertical_gradient(Matrix* view_matrix, CRGB color_top, CRGB color_bottom) {
	StaticPatterns::vertical_gradient(view_matrix, color_top, color_bottom, 0, view_matrix->height);
}

void StaticPatterns::vertical_gradient(Matrix* view_matrix, CRGB color_top, CRGB color_bottom, int32_t y_start, int32_t y_end) {
	int32_t d_height = y_end - y_start;
	float factor_r = ((int32_t) color_bottom.r - (int32_t) color_top.r) / d_height;
	float factor_g = ((int32_t) color_bottom.g - (int32_t) color_top.g) / d_height;
	float factor_b = ((int32_t) color_bottom.b - (int32_t) color_top.b) / d_height;

	float curr_y_color_r = color_top.r;
	float curr_y_color_g = color_top.g;
	float curr_y_color_b = color_top.b;
	CRGB curr_y_color = color_top;

	for (int32_t y = y_start; y < y_end; y++) {
		for (int32_t x = 0; x < view_matrix->width; x++) {
			view_matrix->set(x, y, curr_y_color);
		}
		curr_y_color_r += factor_r;
		curr_y_color_g += factor_g;
		curr_y_color_b += factor_b;
		curr_y_color = CRGB(curr_y_color_r, curr_y_color_g, curr_y_color_b);
	}
}


void StaticPatterns::horizontal_gradient_from_center(Matrix* view_matrix, CRGB color_inner, CRGB color_outer) {

}

void StaticPatterns::fifty_fifty(Matrix* view_matrix, CRGB color_left, CRGB color_right) {
	for(int32_t y = 0; y < view_matrix->height; y++)
		for(int32_t x = 0; x < view_matrix->width; x++) {
			if (x < view_matrix->width / 2) {
				view_matrix->set_absolute(x, y, color_left);
			} else {
				view_matrix->set_absolute(x, y, color_right);
			}
		}
}
