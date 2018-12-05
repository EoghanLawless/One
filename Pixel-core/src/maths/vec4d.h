#pragma once

#include <iostream>

namespace pixel {
	namespace maths {
		struct vec4d {
			float x, y, z, w;

			vec4d() = default;
			vec4d(const float& x, const float& y, const float& z, const float& w);

			vec4d& add(const vec4d& other);
			vec4d& sub(const vec4d& other);
			vec4d& mult(const vec4d& other);
			vec4d& div(const vec4d& other);
			bool equals(const vec4d& other);

			friend vec4d& operator+(vec4d& left, const vec4d& right);
			friend vec4d& operator-(vec4d& left, const vec4d& right);
			friend vec4d& operator*(vec4d& left, const vec4d& right);
			friend vec4d& operator/(vec4d& left, const vec4d& right);

			vec4d& operator+=(const vec4d& other);
			vec4d& operator-=(const vec4d& other);
			vec4d& operator*=(const vec4d& other);
			vec4d& operator/=(const vec4d& other);
			bool operator==(const vec4d& other);
			bool operator!=(const vec4d& other);

			friend std::ostream& operator<<(std::ostream& stream, const vec4d& vector);
		};
	}
}