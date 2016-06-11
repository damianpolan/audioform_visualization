#include "matrix.h"

Matrix::Matrix(uint16_t width, uint16_t height) {
	// initializes the matrix to all black
	this->matrix2D = new CRGB*[height];
	for (uint16_t y = 0; y < height; y++) {
		this->matrix2D[y] = new CRGB[width];
		for (uint16_t x = 0; x < width; x++) {
			this->matrix2D[y][x] = CRGB(0,0,0);
		}
	}
}

CRGB Matrix::get(uint16_t x, uint16_t y) {
	if (x >= 0 && x < this->width
		&& y >= 0 && y < this-> height) {
		return this->matrix2D[y][x];
	} else {
		return CRGB(0, 0, 0);
	}
}

CRGB Matrix::set(uint16_t x, uint16_t y, CRGB color) {
	if (Tools::in_bounds(x, y, this->width, this->height)) {
		return this->matrix2D[y][x];
	}
}

Matrix* Matrix::copy() {
	Matrix* new_one = new Matrix(this->width, this->height);
	for (uint16_t y = 0; y < this->height; y++) {
		for (uint16_t x = 0; x < this->width; x++) {
			new_one->set(x, y, this->matrix2D[y][x]);
		}
	}
	return new_one;
}