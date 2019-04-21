#include "maths.h"

float radians(float degrees) {
	return degrees * (M_PI / 180.0f);
}

void projectPixelCoords(double& x, double& y, const unsigned int pw, const unsigned int ph, const double ww, const double wh) {
	x = (double)((x / pw) * ww) - (ww / 2);
	y = (double)wh - ((y / ph) * wh) - (wh / 2);
}