#include "vec4d.h"

namespace pixel {
	namespace maths {

		vec4d::vec4d(const float& x, const float& y, const float& z, const float& w) {
			this->x = x;
			this->y = y;
			this->z = z;
			this->w = w;
		}

		vec4d& vec4d::add(const vec4d& other) {
			x += other.x;
			y += other.y;
			z += other.z;
			w += other.w;

			return *this;
		}
		vec4d& vec4d::sub(const vec4d& other) {
			x -= other.x;
			y -= other.y;
			z -= other.z;
			w -= other.w;

			return *this;
		}
		vec4d& vec4d::mult(const vec4d& other) {
			x *= other.x;
			y *= other.y;
			z *= other.z;
			w *= other.w;

			return *this;
		}
		vec4d& vec4d::div(const vec4d& other) {
			x /= other.x;
			y /= other.y;
			z /= other.z;
			w /= other.w;

			return *this;
		}
		bool vec4d::equals(const vec4d& other) {
			return (x == other.x && y == other.y && z == other.z && w == other.w);
		}

		vec4d& operator+(vec4d& left, const vec4d& right) {
			return left.add(right);
		}
		vec4d& operator-(vec4d& left, const vec4d& right) {
			return left.sub(right);
		}
		vec4d& operator*(vec4d& left, const vec4d& right) {
			return left.mult(right);
		}
		vec4d& operator/(vec4d& left, const vec4d& right) {
			return left.div(right);
		}

		vec4d& vec4d::operator+=(const vec4d& other) {
			return add(other);
		}
		vec4d& vec4d::operator-=(const vec4d& other) {
			return sub(other);
		}
		vec4d& vec4d::operator*=(const vec4d& other) {
			return mult(other);
		}
		vec4d& vec4d::operator/=(const vec4d& other) {
			return div(other);
		}
		bool vec4d::operator==(const vec4d& other) {
			return equals(other);
		}
		bool vec4d::operator!=(const vec4d& other) {
			return !equals(other);
		}

		std::ostream& operator<<(std::ostream& stream, const vec4d& vector) {
			stream << "vec4d(" << vector.x << "," << vector.y << "," << vector.z << "," << vector.w << ")";
			return stream;
		}
	};
}