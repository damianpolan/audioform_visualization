#include "mixer.h"
#include <stdlib.h> 

void Mixer::add(Matrix* target, Matrix* sourceA, Matrix* sourceB) {
	for (uint16_t y = 0; y < target->height; y++) {
		for (uint16_t x = 0; x < target->width; x++) {
			target->set(x, y, sourceA->get(x, y) + sourceB->get(x, y));
		}
	}
}

void Mixer::subtract(Matrix* target, Matrix* sourceA, Matrix* sourceB) {
	for (uint16_t y = 0; y < target->height; y++) {
		for (uint16_t x = 0; x < target->width; x++) {
			target->set(x, y, sourceA->get(x, y) - sourceB->get(x, y));
		}
	}
}

void Mixer::multiply(Matrix* target, Matrix* sourceA, Matrix* sourceB) {
	for (uint16_t y = 0; y < target->height; y++) {
		for (uint16_t x = 0; x < target->width; x++) {
			CRGB colorA = sourceA->get(x, y);
			CRGB colorB = sourceB->get(x, y);
			CRGB newColor = (colorA[0] * colorB[0], 
				colorA[1] * colorB[1],
				colorA[2] * colorB[2]);
			target->set(x, y, newColor);
		}
	}
}

void Mixer::divide(Matrix* target, Matrix* sourceA, Matrix* sourceB) {
	for (uint16_t y = 0; y < target->height; y++) {
		for (uint16_t x = 0; x < target->width; x++) {
			CRGB colorA = sourceA->get(x, y);
			CRGB colorB = sourceB->get(x, y);
			CRGB newColor = (colorA[0] / colorB[0], 
				colorA[1] / colorB[1],
				colorA[2] / colorB[2]);
			target->set(x, y, newColor);
		}
	}
}


void Mixer::difference(Matrix* target, Matrix* sourceA, Matrix* sourceB) {
	for (uint16_t y = 0; y < target->height; y++) {
		for (uint16_t x = 0; x < target->width; x++) {
			CRGB colorA = sourceA->get(x, y);
			CRGB colorB = sourceB->get(x, y);
			CRGB newColor = (abs(colorA[0] - colorB[0]), 
				abs(colorA[1] / colorB[1]),
				abs(colorA[2] / colorB[2]));
			target->set(x, y, newColor);
		}
	}
}

void Mixer::minv(Matrix* target, Matrix* sourceA, Matrix* sourceB) {
	for (uint16_t y = 0; y < target->height; y++) {
		for (uint16_t x = 0; x < target->width; x++) {
			CRGB colorA = sourceA->get(x, y);
			CRGB colorB = sourceB->get(x, y);
			CRGB newColor = (min(colorA[0], colorB[0]), 
				min(colorA[1], colorB[1]),
				min(colorA[2], colorB[2]));
			target->set(x, y, newColor);
		}
	}
}

void Mixer::maxv(Matrix* target, Matrix* sourceA, Matrix* sourceB) {
	for (uint16_t y = 0; y < target->height; y++) {
		for (uint16_t x = 0; x < target->width; x++) {
			CRGB colorA = sourceA->get(x, y);
			CRGB colorB = sourceB->get(x, y);
			CRGB newColor = (max(colorA[0], colorB[0]), 
				max(colorA[1], colorB[1]),
				max(colorA[2], colorB[2]));
			target->set(x, y, newColor);
		}
	}
}

void Mixer::intersect(Matrix* target, Matrix* sourceA, Matrix* sourceB) {
	for (uint16_t y = 0; y < target->height; y++) {
		for (uint16_t x = 0; x < target->width; x++) {
			CRGB colorA = sourceA->get(x, y);
			CRGB colorB = sourceB->get(x, y);
			CRGB newColor;

			if ((colorA[0] == 0 && colorA[1] == 0 && colorA[2] == 0)
				|| (colorB[0] == 0 && colorB[1] == 0 && colorB[2] == 0)) {
				newColor = CRGB(0, 0, 0);
			}
			else {
				newColor = sourceA->get(x, y) + sourceB->get(x, y);
			}

			target->set(x, y, newColor);
		}
	}
}


void Mixer::average(Matrix* target, Matrix* sourceA, Matrix* sourceB) {
	for (uint16_t y = 0; y < target->height; y++) {
		for (uint16_t x = 0; x < target->width; x++) {
			CRGB colorA = sourceA->get(x, y);
			CRGB colorB = sourceB->get(x, y);
			CRGB newColor = ((colorA[0] + colorB[0]) / 2,
				(colorA[1] + colorB[1]) / 2,
				(colorA[2] + colorB[2]) / 2);
			target->set(x, y, newColor);
		}
	}
}

void Mixer::AND(Matrix* target, Matrix* sourceA, Matrix* sourceB) {
	for (uint16_t y = 0; y < target->height; y++) {
		for (uint16_t x = 0; x < target->width; x++) {
			CRGB colorA = sourceA->get(x, y);
			CRGB colorB = sourceB->get(x, y);
			CRGB newColor = CRGB(0, 0, 0);
			if(colorB[0] != 0 && colorB[1] != 0 && colorB[2] != 0) {
				newColor = colorA;
			}
			target->set(x, y, newColor);
		}
	}
}


