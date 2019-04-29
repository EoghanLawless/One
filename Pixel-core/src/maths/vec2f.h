#pragma once

#include <iostream>

namespace pixel {
	namespace maths {

		struct vec2f {
			float x, y;

			vec2f();
			vec2f(const float& x, const float& y);

			vec2f& set(const float x, const float y);
			vec2f& set(const vec2f v);

			vec2f& add(const vec2f& other);
			vec2f& add(const float value);
			vec2f& sub(const vec2f& other);
			vec2f& sub(const float value);
			vec2f& mult(const vec2f& other);
			vec2f& mult(const float multiplier);
			vec2f& div(const vec2f& other);
			vec2f& div(const float divisor);
			bool equals(const vec2f& other);

			vec2f& normalise();

			friend vec2f& operator+(vec2f& left, const vec2f& right);
			friend vec2f& operator-(vec2f& left, const vec2f& right);
			friend vec2f& operator*(vec2f& left, const vec2f& right);
			friend vec2f& operator/(vec2f& left, const vec2f& right);

			vec2f& operator+=(const vec2f& other);
			vec2f& operator-=(const vec2f& other);
			vec2f& operator*=(const vec2f& other);
			vec2f& operator/=(const vec2f& other);
			bool operator==(const vec2f& other);
			bool operator!=(const vec2f& other);

			friend std::ostream& operator<<(std::ostream& stream, const vec2f& vector);
		};

	}
}