#pragma once

#include <iostream>
#include <vector>
#include <GL/glew.h>

#include "../maths/maths.h"
#include "../utils/fileutils.h"

namespace pixel {
	namespace graphics {

		class Shader {
		private:
			GLuint _shaderId;

			const char* _vertexPath;
			const char* _fragmentPath;

			GLuint load();
			GLint getUniformLocation(const GLchar* name);
		public:
			Shader(const char* vertexPath, const char* fragmentShader);
			~Shader();

			void setUniform1f(const GLchar* name, float value);
			void setUniform1i(const GLchar* name, int value);
			void setUniform2f(const GLchar* name, const maths::vec2d& vector);
			void setUniform3f(const GLchar* name, const maths::vec3d& vector);
			void setUniform4f(const GLchar* name, const maths::vec4d& vector);
			void setUniformMat4(const GLchar* name, const maths::mat4& matrix);

			void enable() const;
			void disable() const;
		};
	}
}