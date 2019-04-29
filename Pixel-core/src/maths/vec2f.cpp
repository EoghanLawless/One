#include "vec2f.h"

namespace pixel {
	namespace maths {

		vec2f::vec2f() {
			x = 0.0f;
			y = 0.0f;
		}
		vec2f::vec2f(const float& x, const float& y) {
			this->x = x;
			this->y = y;
		}

		vec2f& vec2f::set(const float x, const float y) {
			this->x = x;
			this->y = y;

			return *this;
		}
		vec2f& vec2f::set(const vec2f v) {
			x = v.x;
			y = v.y;

			return *this;
		}

		vec2f& vec2f::add(const vec2f& other) {
			x += other.x;
			y += other.y;

			return *this;
		}
		vec2f& vec2f::add(const float value) {
			x += value;
			y += value;

			return *this;
		}
		vec2f& vec2f::sub(const vec2f& other) {
			x -= other.x;
			y -= other.y;

			return *this;
		}
		vec2f& vec2f::sub(const float value) {
			x -= value;
			y -= value;

			return *this;
		}
		vec2f& vec2f::mult(const vec2f& other) {
			x *= other.x;
			y *= other.y;

			return *this;
		}
		vec2f& vec2f::mult(const float multiplier) {
			x *= multiplier;
			y *= multiplier;

			return *this;
		}
		vec2f& vec2f::div(const vec2f& other) {
			x /= other.x;
			y /= other.y;

			return *this;
		}
		vec2f& vec2f::div(const float divisor) {
			x /= divisor;
			y /= divisor;

			return *this;
		}
		bool vec2f::equals(const vec2f& other) {
			return (x == other.x && y == other.y);
		}

		vec2f& vec2f::normalise() {
			float mag = sqrt((x*x) + (y*y));
			this->div(mag);
			return *this;
		}

		vec2f& operator+(vec2f& left, const vec2f& right) {
			return left.add(right);
		}
		vec2f& operator-(vec2f& left, const vec2f& right) {
			return left.sub(right);
		}
		vec2f& operator*(vec2f& left, const vec2f& right) {
			return left.mult(right);
		}
		vec2f& operator/(vec2f& left, const vec2f& right) {
			return left.div(right);
		}

		vec2f& vec2f::operator+=(const vec2f& other) {
			return add(other);
		}
		vec2f& vec2f::operator-=(const vec2f& other) {
			return sub(other);
		}
		vec2f& vec2f::operator*=(const vec2f& other) {
			return mult(other);
		}
		vec2f& vec2f::operator/=(const vec2f& other) {
			return div(other);
		}
		bool vec2f::operator==(const vec2f& other) {
			return equals(other);
		}
		bool vec2f::operator!=(const vec2f& other) {
			return !equals(other);
		}

		std::ostream& operator<<(std::ostream& stream, const vec2f& vector) {
			stream << "vec2f(" << vector.x << "," << vector.y << ")";
			return stream;
		}

	}
}