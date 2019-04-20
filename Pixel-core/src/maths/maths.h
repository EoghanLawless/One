#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include "vec2f.h"
#include "vec3f.h"
#include "vec4f.h"
#include "mat4.h"

float radians(float degrees);
void projectPixelCoords(double& x, double& y, const unsigned int ww, const unsigned int wh, const double pw, const double ph);