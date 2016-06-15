#include "mixer.h"

void Mixer::add(Matrix* target, Matrix* sourceA, Matrix* sourceB) {
	for (int32_t y = 0; y < target->height; y++) {
		for (int32_t x = 0; x < target->width; x++) {
			target->set(x, y, sourceA->get(x, y) + sourceB->get(x, y));
		}
	}
}

void Mixer::add_weighted(Matrix* target, Matrix* sourceA, Matrix* sourceB, float sourceA_weight, float sourceB_weight) {
	for (int32_t y = 0; y < target->height; y++) {
		for (int32_t x = 0; x < target->width; x++) {
			CRGB targA = sourceA->get(x, y);
			targA[0] *= sourceA_weight;
			targA[1] *= sourceA_weight;
			targA[2] *= sourceA_weight;
			
			CRGB targB = sourceB->get(x, y);
			targB[0] *= sourceB_weight;
			targB[1] *= sourceB_weight;
			targB[2] *= sourceB_weight;

			target->set(x, y, targA + targB);
		}
	}
}

void Mixer::subtract(Matrix* target, Matrix* sourceA, Matrix* sourceB) {
	for (int32_t y = 0; y < target->height; y++) {
		for (int32_t x = 0; x < target->width; x++) {
			target->set(x, y, sourceA->get(x, y) - sourceB->get(x, y));
		}
	}
}

void Mixer::multiply(Matrix* target, Matrix* sourceA, Matrix* sourceB) {
	for (int32_t y = 0; y < target->height; y++) {
		for (int32_t x = 0; x < target->width; x++) {
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
	for (int32_t y = 0; y < target->height; y++) {
		for (int32_t x = 0; x < target->width; x++) {
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
	for (int32_t y = 0; y < target->height; y++) {
		for (int32_t x = 0; x < target->width; x++) {
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
	for (int32_t y = 0; y < target->height; y++) {
		for (int32_t x = 0; x < target->width; x++) {
			CRGB colorA = sourceA->get(x, y);
			CRGB colorB = sourceB->get(x, y);
			CRGB newColor = (std::min(colorA[0], colorB[0]), 
				std::min(colorA[1], colorB[1]),
				std::min(colorA[2], colorB[2]));
			target->set(x, y, newColor);
		}
	}
}

void Mixer::maxv(Matrix* target, Matrix* sourceA, Matrix* sourceB) {
	for (int32_t y = 0; y < target->height; y++) {
		for (int32_t x = 0; x < target->width; x++) {
			CRGB colorA = sourceA->get(x, y);
			CRGB colorB = sourceB->get(x, y);
			CRGB newColor = (std::max(colorA[0], colorB[0]), 
				std::max(colorA[1], colorB[1]),
				std::max(colorA[2], colorB[2]));
			target->set(x, y, newColor);
		}
	}
}

void Mixer::intersect(Matrix* target, Matrix* sourceA, Matrix* sourceB) {
	for (int32_t y = 0; y < target->height; y++) {
		for (int32_t x = 0; x < target->width; x++) {
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
	for (int32_t y = 0; y < target->height; y++) {
		for (int32_t x = 0; x < target->width; x++) {
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
	for (int32_t y = 0; y < target->height; y++) {
		for (int32_t x = 0; x < target->width; x++) {
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


