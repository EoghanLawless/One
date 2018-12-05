#pragma once

#include <iostream>

namespace pixel {
	namespace maths {
		struct vec2d {
			float x, y;

			vec2d();
			vec2d(const float& x, const float& y);

			vec2d& add(const vec2d& other);
			vec2d& sub(const vec2d& other);
			vec2d& mult(const vec2d& other);
			vec2d& div(const vec2d& other);
			bool equals(const vec2d& other);

			friend vec2d& operator+(vec2d& left, const vec2d& right);
			friend vec2d& operator-(vec2d& left, const vec2d& right);
			friend vec2d& operator*(vec2d& left, const vec2d& right);
			friend vec2d& operator/(vec2d& left, const vec2d& right);

			vec2d& operator+=(const vec2d& other);
			vec2d& operator-=(const vec2d& other);
			vec2d& operator*=(const vec2d& other);
			vec2d& operator/=(const vec2d& other);
			bool operator==(const vec2d& other);
			bool operator!=(const vec2d& other);

			friend std::ostream& operator<<(std::ostream& stream, const vec2d& vector);
		};
	}
}