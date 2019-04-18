#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <GL/glew.h>

#include "maths/maths.h"
#include "utils/fileutils.h"

namespace pixel {
	namespace graphics {

		class Shader {
		private:
			GLuint _shaderId;

			std::string _vertexPath;
			std::string _fragmentPath;

			GLuint load();
			GLint getUniformLocation(const GLchar* name);

		public:
			Shader(std::string vertexPath, std::string fragmentShader);
			~Shader();

			void setUniform1f(const GLchar* name, float value);
			void setUniform1fv(const GLchar* name, float* value, int count);
			void setUniform1i(const GLchar* name, int value);
			void setUniform1iv(const GLchar* name, int* value, int count);
			void setUniform2f(const GLchar* name, const maths::vec2f& vector);
			void setUniform3f(const GLchar* name, const maths::vec3f& vector);
			void setUniform4f(const GLchar* name, const maths::vec4f& vector);
			void setUniformMat4(const GLchar* name, const maths::mat4& matrix);

			void enable() const;
			void disable() const;
		};

	}
}