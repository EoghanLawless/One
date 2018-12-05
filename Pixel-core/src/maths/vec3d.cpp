#include "vec3d.h"

namespace pixel {
	namespace maths {

		vec3d::vec3d() {
			x = 0.0f;
			y = 0.0f;
			z = 0.0f;
		}

		vec3d::vec3d(const float& x, const float& y, const float& z) {
			this->x = x;
			this->y = y;
			this->z = z;
		}

		vec3d& vec3d::add(const vec3d& other) {
			x += other.x;
			y += other.y;
			z += other.z;

			return *this;
		}
		vec3d& vec3d::sub(const vec3d& other) {
			x -= other.x;
			y -= other.y;
			z -= other.z;

			return *this;
		}
		vec3d& vec3d::mult(const vec3d& other) {
			x *= other.x;
			y *= other.y;
			z *= other.z;

			return *this;
		}
		vec3d& vec3d::div(const vec3d& other) {
			x /= other.x;
			y /= other.y;
			z /= other.z;

			return *this;
		}
		bool vec3d::equals(const vec3d& other) {
			return (x == other.x && y == other.y && z == other.z);
		}

		vec3d& operator+(vec3d& left, const vec3d& right) {
			return left.add(right);
		}
		vec3d& operator-(vec3d& left, const vec3d& right) {
			return left.sub(right);
		}
		vec3d& operator*(vec3d& left, const vec3d& right) {
			return left.mult(right);
		}
		vec3d& operator/(vec3d& left, const vec3d& right) {
			return left.div(right);
		}

		vec3d& vec3d::operator+=(const vec3d& other) {
			return add(other);
		}
		vec3d& vec3d::operator-=(const vec3d& other) {
			return sub(other);
		}
		vec3d& vec3d::operator*=(const vec3d& other) {
			return mult(other);
		}
		vec3d& vec3d::operator/=(const vec3d& other) {
			return div(other);
		}
		bool vec3d::operator==(const vec3d& other) {
			return equals(other);
		}
		bool vec3d::operator!=(const vec3d& other) {
			return !equals(other);
		}

		std::ostream& operator<<(std::ostream& stream, const vec3d& vector) {
			stream << "vec3d(" << vector.x << "," << vector.y << "," << vector.z << ")";
			return stream;
		}
	};
}