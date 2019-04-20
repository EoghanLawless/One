#pragma once

#include <iostream>

namespace pixel {
	namespace maths {

		struct vec4f {
			float x, y, z, w;

			vec4f();
			vec4f(const float& x, const float& y, const float& z, const float& w);

			vec4f& set(const float x, const float y, const float z, const float w);
			vec4f& set(const vec4f v);

			vec4f& add(const vec4f& other);
			vec4f& sub(const vec4f& other);
			vec4f& mult(const vec4f& other);
			vec4f& div(const vec4f& other);
			bool equals(const vec4f& other);

			friend vec4f& operator+(vec4f& left, const vec4f& right);
			friend vec4f& operator-(vec4f& left, const vec4f& right);
			friend vec4f& operator*(vec4f& left, const vec4f& right);
			friend vec4f& operator/(vec4f& left, const vec4f& right);

			vec4f& operator+=(const vec4f& other);
			vec4f& operator-=(const vec4f& other);
			vec4f& operator*=(const vec4f& other);
			vec4f& operator/=(const vec4f& other);
			bool operator==(const vec4f& other);
			bool operator!=(const vec4f& other);

			friend std::ostream& operator<<(std::ostream& stream, const vec4f& vector);
		};

	}
}