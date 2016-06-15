#include "matrix.h"

Matrix::Matrix(int32_t width, int32_t height) {
	// initializes the matrix to all black
	this->matrix2D.resize(height, std::vector<CRGB>(width, CRGB::Red));
	this->width = width;
	this->height = height;
	/*this->matrix2D.resize(height);
	for (int32_t y = 0; y < height; y++) {
		this->matrix2D[y].resize(width);
	}*/
	/*this->matrix2D = new CRGB*[height];
	for (int32_t y = 0; y < height; y++) {
		this->matrix2D[y] = new CRGB[width];
		for (int32_t x = 0; x < width; x++) {
			this->matrix2D[y][x] = CRGB(0,0,0);
		}
	}*/
}

CRGB Matrix::get(int32_t x, int32_t y) {

	if (x >= 0 && x < this->width
		&& y >= 0 && y < this->height) {
		return this->matrix2D.at(y).at(x);
	} else {
		return CRGB(0, 0, 0);
	}
}

void Matrix::set(int32_t x, int32_t y, CRGB color) {
	Serial.print("asds");
	if (Tools::in_bounds(x, y, this->width, this->height)) {
		Serial.print("asds2");
		this->matrix2D[y][x] = color;
		Serial.print("asds3");
	}
}


void Matrix::clear(CRGB color) {
	for (int32_t y = 0; y < this->height; y++) {
		for (int32_t x = 0; x < this->width; x++) {
			this->matrix2D[y][x] = color;
		}
	}
}

Matrix* Matrix::copy() {
	Matrix* new_one = new Matrix(this->width, this->height);
	for (int32_t y = 0; y < this->height; y++) {
		for (int32_t x = 0; x < this->width; x++) {
			new_one->set(x, y, this->matrix2D[y][x]);
		}
	}
	return new_one;
}


namespace std {
	void __throw_bad_alloc()
	{
		Serial.println("Unable to allocate memory");
	}

	void __throw_length_error(char const*e)
	{
		Serial.print("Length Error :");
		Serial.println(e);
	}


	void __throw_out_of_range(char const*e)
	{
		Serial.print("Length Error :");
		Serial.println(e);
	}
}