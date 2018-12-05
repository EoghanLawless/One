#pragma once

#include <iostream>

namespace pixel {
	namespace maths {
		struct vec3d {
			float x, y, z;

			vec3d();
			vec3d(const float& x, const float& y, const float& z);

			vec3d& add(const vec3d& other);
			vec3d& sub(const vec3d& other);
			vec3d& mult(const vec3d& other);
			vec3d& div(const vec3d& other);
			bool equals(const vec3d& other);

			friend vec3d& operator+(vec3d& left, const vec3d& right);
			friend vec3d& operator-(vec3d& left, const vec3d& right);
			friend vec3d& operator*(vec3d& left, const vec3d& right);
			friend vec3d& operator/(vec3d& left, const vec3d& right);

			vec3d& operator+=(const vec3d& other);
			vec3d& operator-=(const vec3d& other);
			vec3d& operator*=(const vec3d& other);
			vec3d& operator/=(const vec3d& other);
			bool operator==(const vec3d& other);
			bool operator!=(const vec3d& other);

			friend std::ostream& operator<<(std::ostream& stream, const vec3d& vector);
		};
	}
}