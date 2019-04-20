#include "vec3f.h"

namespace pixel {
	namespace maths {

		vec3f::vec3f() {
			x = 0.0f;
			y = 0.0f;
			z = 0.0f;
		}
		vec3f::vec3f(const float& x, const float& y, const float& z) {
			this->x = x;
			this->y = y;
			this->z = z;
		}

		vec3f& vec3f::set(const float x, const float y, const float z) {
			this->x = x;
			this->y = y;
			this->z = z;

			return *this;
		}
		vec3f& vec3f::set(const vec3f v) {
			x = v.x;
			y = v.y;
			z = v.z;

			return *this;
		}

		vec3f& vec3f::add(const vec3f& other) {
			x += other.x;
			y += other.y;
			z += other.z;

			return *this;
		}
		vec3f& vec3f::sub(const vec3f& other) {
			x -= other.x;
			y -= other.y;
			z -= other.z;

			return *this;
		}
		vec3f& vec3f::mult(const vec3f& other) {
			x *= other.x;
			y *= other.y;
			z *= other.z;

			return *this;
		}
		vec3f& vec3f::div(const vec3f& other) {
			x /= other.x;
			y /= other.y;
			z /= other.z;

			return *this;
		}
		bool vec3f::equals(const vec3f& other) {
			return (x == other.x && y == other.y && z == other.z);
		}

		vec3f& operator+(vec3f& left, const vec3f& right) {
			return left.add(right);
		}
		vec3f& operator-(vec3f& left, const vec3f& right) {
			return left.sub(right);
		}
		vec3f& operator*(vec3f& left, const vec3f& right) {
			return left.mult(right);
		}
		vec3f& operator/(vec3f& left, const vec3f& right) {
			return left.div(right);
		}

		vec3f& vec3f::operator+=(const vec3f& other) {
			return add(other);
		}
		vec3f& vec3f::operator-=(const vec3f& other) {
			return sub(other);
		}
		vec3f& vec3f::operator*=(const vec3f& other) {
			return mult(other);
		}
		vec3f& vec3f::operator/=(const vec3f& other) {
			return div(other);
		}
		bool vec3f::operator==(const vec3f& other) {
			return equals(other);
		}
		bool vec3f::operator!=(const vec3f& other) {
			return !equals(other);
		}

		std::ostream& operator<<(std::ostream& stream, const vec3f& vector) {
			stream << "vec3f(" << vector.x << "," << vector.y << "," << vector.z << ")";
			return stream;
		}

	}
}