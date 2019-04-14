#pragma once

#include <iostream>

namespace pixel {
	namespace maths {

		struct vec3f {
			float x, y, z;

			vec3f();
			vec3f(const float& x, const float& y, const float& z);

			vec3f& add(const vec3f& other);
			vec3f& sub(const vec3f& other);
			vec3f& mult(const vec3f& other);
			vec3f& div(const vec3f& other);
			bool equals(const vec3f& other);

			friend vec3f& operator+(vec3f& left, const vec3f& right);
			friend vec3f& operator-(vec3f& left, const vec3f& right);
			friend vec3f& operator*(vec3f& left, const vec3f& right);
			friend vec3f& operator/(vec3f& left, const vec3f& right);

			vec3f& operator+=(const vec3f& other);
			vec3f& operator-=(const vec3f& other);
			vec3f& operator*=(const vec3f& other);
			vec3f& operator/=(const vec3f& other);
			bool operator==(const vec3f& other);
			bool operator!=(const vec3f& other);

			friend std::ostream& operator<<(std::ostream& stream, const vec3f& vector);
		};

	}
}