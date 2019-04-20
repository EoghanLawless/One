#include "maths.h"

float radians(float degrees) {
	return degrees * (M_PI / 180.0f);
}

void projectPixelCoords(double& x, double& y, const unsigned int ww, const unsigned int wh, const double pw, const double ph) {
	x = (double)(((x * pw) / ww) - pw / 2);
	y = (double)(ph / 2 - ((y * ph) / wh));
}