#include "matrix.h"

Matrix::Matrix(int32_t width, int32_t height) {
	// initializes the matrix to all black
	this->matrix2D.resize(height, std::vector<CRGB>(width, CRGB::Black));
	this->width = width;
	this->height = height;
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
	if (Tools::in_bounds(x, y, this->width, this->height)) {
		this->matrix2D[y][x] = color;
	}
}


void Matrix::clear(CRGB color) {
	for (int32_t y = 0; y < this->height; y++) {
		for (int32_t x = 0; x < this->width; x++) {
			this->matrix2D[y][x] = color;
		}
	}
}

Matrix* Matrix::half(Point subbox_p0, Point subbox_p1) {
	int32_t width = (subbox_p1.x - subbox_p0.x) / 2;
	int32_t height = (subbox_p1.y - subbox_p0.y) / 2;
	Matrix* matrix_reduced = new Matrix(width, height);
	
	for (int32_t y = 0; y < height; y++) {
		for (int32_t x = 0; x < width; x++) {
			CRGB c0 = this->matrix2D[subbox_p0.y + 2*y][subbox_p0.x + 2*x];
			CRGB c1 = this->matrix2D[subbox_p0.y + 2*y][subbox_p0.x + 2*x + 1];
			CRGB c2 = this->matrix2D[subbox_p0.y + 2*y + 1][subbox_p0.x + 2*x];
			CRGB c3 = this->matrix2D[subbox_p0.y + 2*y + 1][subbox_p0.x + 2*x + 1];

			int32_t red = (c0.r + c1.r + c2.r + c3.r) / 4;
			int32_t green = (c0.g + c1.g + c2.g + c3.g) / 4;
			int32_t blue = (c0.b + c1.b + c2.b + c3.b) / 4;

			matrix_reduced->set(x, y, CRGB(red, green, blue));
		}	
	}	

 // for (int i = 0; i < height; ++i)
 //  {
 //      for (int j = 0; j < width; ++j)
 //      {
 //          Serial.print(this->get(i, j).r);
 //          Serial.print(" ");
 //      }
 //      Serial.print("\n\r");
 //  }

	return matrix_reduced;
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