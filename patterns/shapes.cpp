#include "shapes.h"


// HANDLE CASE FOR VERTICAL LINE
//void Shapes::line(Matrix* target, Point p0, Point p1, CRGB color) {
//
//	float deltax = p1.x - p0.x;
//
//	if (deltax == 0) {
//		int32_t diff = 1;
//		if (p0.y > p1.y) {
//			diff = -1;
//		}
//		for (int32_t y = p0.y; y < p1.y; y += diff) {
//			target->set(p0.x, y, color);
//		}
//		return;
//	}
//	float deltay = p1.y - p0.y;
//	float error = -1.0;
//	float deltaerr = abs(deltay / deltax);    // Assume deltax != 0 (line is not vertical)
//	int32_t y = p0.y;
//
//	for (int32_t x = p0.x; x < p1.x; x++) {
//		target->set(x, y, color);
//		error = error + deltaerr;
//		if (error >= 0.0) {
//			y = y + 1;
//			error = error - 1.0;
//		}
//	}
//}

void Shapes::line(Matrix* target, Point p0, Point p1, CRGB color)
{
	// Bresenham's line algorithm
	bool steep = (abs(p1.y - p0.y) > abs(p1.x - p0.x));
	if(steep)
	{
		std::swap(p0.x, p0.y);
		std::swap(p1.x, p1.y);
	}

	if(p0.x > p1.x)
	{
		std::swap(p0.x, p1.x);
		std::swap(p0.y, p1.y);
	}

	int32_t dx = p1.x - p0.x;
	int32_t dy = abs(p1.y - p0.y);

	float error = dx / 2.0f;
	int32_t ystep = (p0.y < p1.y) ? 1 : -1;
	int32_t y = p0.y;

	for(int32_t x = p0.x; x <= p1.x; x++)
	{
		if(steep)
		{
			target->set(y, x, color);
		}
		else
		{
			target->set(x, y, color);
		}

		error -= dy;
		if(error < 0)
		{
			y += ystep;
			error += dx;
		}
	}
}


void Shapes::circle(Matrix* target, Point p0, int32_t radius, CRGB color) {
	int32_t x = radius;
	int32_t y = 0;
	int32_t err = 0;

	while (x >= y) {
		target->set(p0.x + x, p0.y + y, color);
		target->set(p0.x + y, p0.y + x, color);
		target->set(p0.x - y, p0.y + x, color);
		target->set(p0.x - x, p0.y + y, color);
		target->set(p0.x - x, p0.y - y, color);
		target->set(p0.x - y, p0.y - x, color);
		target->set(p0.x + y, p0.y - x, color);
		target->set(p0.x + x, p0.y - y, color);

		y += 1;
		err += 1 + 2 * y;
		if (2 * (err - x) + 1 > 0) {
			x -= 1;
			err += 1 - 2 * x;
		}
	}
}

void Shapes::rectangle(Matrix* target, Point p0, Point p2, CRGB color) {
	Point p1 = Point(p2.x, p0.y);
	Point p3 = Point(p0.x, p2.y);

	Shapes::line(target, p0, p1, color);
	Shapes::line(target, p1, p2, color);
	Shapes::line(target, p2, p3, color);
	Shapes::line(target, p3, p0, color);
}

void Shapes::rectangle(Matrix* target, Point p0, int32_t width, int32_t height, CRGB color) {
	Point p2 = Point(p0.x + width, p0.y + height);
	Shapes::rectangle(target, p0, p2, color);
}


// void Shapes::poly(Matrix* target, Point p0, Point p1, Point p2, CRGB color) {

// }