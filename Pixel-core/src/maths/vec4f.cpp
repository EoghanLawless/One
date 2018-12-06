#include "vec4f.h"

namespace pixel {
	namespace maths {

		vec4f::vec4f(const float& x, const float& y, const float& z, const float& w) {
			this->x = x;
			this->y = y;
			this->z = z;
			this->w = w;
		}

		vec4f& vec4f::add(const vec4f& other) {
			x += other.x;
			y += other.y;
			z += other.z;
			w += other.w;

			return *this;
		}
		vec4f& vec4f::sub(const vec4f& other) {
			x -= other.x;
			y -= other.y;
			z -= other.z;
			w -= other.w;

			return *this;
		}
		vec4f& vec4f::mult(const vec4f& other) {
			x *= other.x;
			y *= other.y;
			z *= other.z;
			w *= other.w;

			return *this;
		}
		vec4f& vec4f::div(const vec4f& other) {
			x /= other.x;
			y /= other.y;
			z /= other.z;
			w /= other.w;

			return *this;
		}
		bool vec4f::equals(const vec4f& other) {
			return (x == other.x && y == other.y && z == other.z && w == other.w);
		}

		vec4f& operator+(vec4f& left, const vec4f& right) {
			return left.add(right);
		}
		vec4f& operator-(vec4f& left, const vec4f& right) {
			return left.sub(right);
		}
		vec4f& operator*(vec4f& left, const vec4f& right) {
			return left.mult(right);
		}
		vec4f& operator/(vec4f& left, const vec4f& right) {
			return left.div(right);
		}

		vec4f& vec4f::operator+=(const vec4f& other) {
			return add(other);
		}
		vec4f& vec4f::operator-=(const vec4f& other) {
			return sub(other);
		}
		vec4f& vec4f::operator*=(const vec4f& other) {
			return mult(other);
		}
		vec4f& vec4f::operator/=(const vec4f& other) {
			return div(other);
		}
		bool vec4f::operator==(const vec4f& other) {
			return equals(other);
		}
		bool vec4f::operator!=(const vec4f& other) {
			return !equals(other);
		}

		std::ostream& operator<<(std::ostream& stream, const vec4f& vector) {
			stream << "vec4f(" << vector.x << "," << vector.y << "," << vector.z << "," << vector.w << ")";
			return stream;
		}
	};
}