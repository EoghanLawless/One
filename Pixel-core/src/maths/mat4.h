#pragma once

#include "maths.h"

namespace pixel {
	namespace maths {

		struct mat4 {
			union {
				float elements[4 * 4];
				vec4f columns[4];
			};

			mat4();
			mat4(float diagonal);

			static mat4 identity();

			mat4& mult(const mat4& other);
			friend mat4 operator*(mat4 left, const mat4& right);
			mat4& operator*=(const mat4& other);

			vec3f mult(const vec3f& other) const;
			friend vec3f operator*(const mat4& left, const vec3f& right);

			vec4f mult(const vec4f& other) const;
			friend vec4f operator*(const mat4& left, const vec4f& right);

			static mat4 orthographic(float left, float right, float bottom, float top, float near, float far);
			static mat4 perspective(float fov, float aratio, float near, float far);

			static mat4 translation(const vec3f& translation);
			static mat4 rotation(float angle, const vec3f& axis);
			static mat4 scale(const vec3f& scale);

			friend std::ostream& operator<<(std::ostream& stream, const mat4& matrix);
		};

	}
}