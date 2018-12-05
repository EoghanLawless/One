#include "vec2d.h"

namespace pixel {
	namespace maths {

		vec2d::vec2d() {
			x = 0.0f;
			y = 0.0f;
		}

		vec2d::vec2d(const float& x, const float& y) {
			this->x = x;
			this->y = y;
		}

		vec2d& vec2d::add(const vec2d& other) {
			x += other.x;
			y += other.y;

			return *this;
		}
		vec2d& vec2d::sub(const vec2d& other) {
			x -= other.x;
			y -= other.y;

			return *this;
		}
		vec2d& vec2d::mult(const vec2d& other) {
			x *= other.x;
			y *= other.y;

			return *this;
		}
		vec2d& vec2d::div(const vec2d& other) {
			x /= other.x;
			y /= other.y;

			return *this;
		}
		bool vec2d::equals(const vec2d& other) {
			return (x == other.x && y == other.y);
		}

		vec2d& operator+(vec2d& left, const vec2d& right) {
			return left.add(right);
		}
		vec2d& operator-(vec2d& left, const vec2d& right) {
			return left.sub(right);
		}
		vec2d& operator*(vec2d& left, const vec2d& right) {
			return left.mult(right);
		}
		vec2d& operator/(vec2d& left, const vec2d& right) {
			return left.div(right);
		}

		vec2d& vec2d::operator+=(const vec2d& other) {
			return add(other);
		}
		vec2d& vec2d::operator-=(const vec2d& other) {
			return sub(other);
		}
		vec2d& vec2d::operator*=(const vec2d& other) {
			return mult(other);
		}
		vec2d& vec2d::operator/=(const vec2d& other) {
			return div(other);
		}
		bool vec2d::operator==(const vec2d& other) {
			return equals(other);
		}
		bool vec2d::operator!=(const vec2d& other) {
			return !equals(other);
		}

		std::ostream& operator<<(std::ostream& stream, const vec2d& vector) {
			stream << "vec2d(" << vector.x << "," << vector.y << ")";
			return stream;
		}
	};
}