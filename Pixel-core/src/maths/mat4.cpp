#include "mat4.h"

namespace pixel {
	namespace maths {

		mat4::mat4() {
			for (int e = 0; e < 4 * 4; e++) {
				elements[e] = 0;
			}
		}
		mat4::mat4(float diagonal) {
			for (int e = 0; e < 4 * 4; e++) {
				elements[e] = 0;
			}

			elements[0 + (0 * 4)] = diagonal;
			elements[1 + (1 * 4)] = diagonal;
			elements[2 + (2 * 4)] = diagonal;
			elements[3 + (3 * 4)] = diagonal;
		}

		mat4 mat4::identity() {
			return mat4(1.0f);
		}

		mat4& mat4::mult(const mat4& other) {
			float data[16];

			for (int y = 0; y < 4; y++) {
				for (int x = 0; x < 4; x++) {
					float sum = 0.0f;	
					for (int e = 0; e < 4; e++) {
						sum += elements[x + (e * 4)] * other.elements[e + (y * 4)];
					}
					data[x + (y * 4)] = sum;
				}
			}

			memcpy(elements, data, 16 * 4);

			return *this;
		}
		mat4 operator*(mat4 left, const mat4& right) {
			return left.mult(right);
		}
		mat4& mat4::operator*=(const mat4& other) {
			return mult(other);
		}

		vec3f mat4::mult(const vec3f& other) const {
			return vec3f(
				columns[0].x * other.x + columns[1].x * other.y + columns[2].x * other.z + columns[3].x,
				columns[0].y * other.x + columns[1].y * other.y + columns[2].y * other.z + columns[3].y,
				columns[0].z * other.x + columns[1].z * other.y + columns[2].z * other.z + columns[3].z
			);
		}
		vec3f operator*(const mat4& left, const vec3f& right) {
			return left.mult(right);
		}

		vec4f mat4::mult(const vec4f& other) const {
			return vec4f(
				columns[0].x * other.x + columns[1].x * other.y + columns[2].x * other.z + columns[3].x * other.w,
				columns[0].y * other.x + columns[1].y * other.y + columns[2].y * other.z + columns[3].y * other.w,
				columns[0].z * other.x + columns[1].z * other.y + columns[2].z * other.z + columns[3].z * other.w,
				columns[0].w * other.x + columns[1].w * other.y + columns[2].w * other.z + columns[3].w * other.w
			);
		}
		vec4f operator*(const mat4& left, const vec4f& right) {
			return left.mult(right);
		}

		mat4 mat4::orthographic(float left, float right, float bottom, float top, float near, float far) {
			mat4 result(1.0f);

			result.elements[0 + (0 * 4)] = 2.0f / (right - left);
			result.elements[1 + (1 * 4)] = 2.0f / (top - bottom);
			result.elements[2 + (2 * 4)] = 2.0f / (near - far);

			result.elements[0 + (3 * 4)] = (left + right) / (left - right);
			result.elements[1 + (3 * 4)] = (bottom + top) / (bottom - top);
			result.elements[2 + (3 * 4)] = (far + near) / (far - near);

			return result;
		}
		mat4 mat4::perspective(float fov, float aratio, float near, float far) {
			mat4 result(0.0f);

			float angle = tan(radians(fov / 2.0f));
			result.elements[0 + (0 * 4)] = 1.0f / (aratio * angle);
			result.elements[1 + (1 * 4)] = 1.0f / angle;
			result.elements[2 + (2 * 4)] = (0 - near - far) / (near - far);
			result.elements[3 + (2 * 4)] = -1.0f;
			result.elements[2 + (3 * 4)] = (2 * far * near) / (near - far);

			return result;
		}

		mat4 mat4::translation(const vec3f& translation) {
			mat4 result(1.0f);

			result.elements[0 + (3 * 4)] = translation.x;
			result.elements[1 + (3 * 4)] = translation.y;
			result.elements[2 + (3 * 4)] = translation.z;

			return result;
		}
		mat4 mat4::rotation(float angle, const vec3f& axis) {
			mat4 result(1.0f);

			float r = radians(angle);
			float c = cos(r);
			float s = sin(r);
			float omc = 1.0f - c;

			float x = axis.x;
			float y = axis.y;
			float z = axis.z;

			result.elements[0 + (0 * 4)] = x * x * omc + c;
			result.elements[1 + (0 * 4)] = y * x * omc + z * s;
			result.elements[2 + (0 * 4)] = x * z * omc - y * s;

			result.elements[0 + (1 * 4)] = x * y * omc - z * s;
			result.elements[1 + (1 * 4)] = y * y * omc + c;
			result.elements[2 + (1 * 4)] = y * z * omc + x * s;

			result.elements[0 + (2 * 4)] = x * z * omc + y * s;
			result.elements[1 + (2 * 4)] = y * z * omc - x * s;
			result.elements[2 + (2 * 4)] = z * z * omc + c;

			return result;
		}
		mat4 mat4::scale(const vec3f& scale) {
			mat4 result(1.0f);

			result.elements[0 + (0 * 4)] = scale.x;
			result.elements[1 + (1 * 4)] = scale.y;
			result.elements[2 + (2 * 4)] = scale.z;

			return result;
		}

		std::ostream& operator<<(std::ostream& stream, const mat4& matrix) {
			for (int col = 0; col < 4; col++) {
				stream << matrix.columns[col].x << " ";
			}
			stream << std::endl;
			for (int col = 0; col < 4; col++) {
				stream << matrix.columns[col].y << " ";
			}
			stream << std::endl;
			for (int col = 0; col < 4; col++) {
				stream << matrix.columns[col].z << " ";
			}
			stream << std::endl;
			for (int col = 0; col < 4; col++) {
				stream << matrix.columns[col].w << " ";
			}
			stream << std::endl;
			return stream;
		}
	}
}