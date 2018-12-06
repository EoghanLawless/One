#pragma once

#include <GL/glew.h>

namespace pixel {
	namespace graphics {

		class Buffer {
		private:
			GLuint _bufferId;
			GLuint _componentCount;

		public:
			Buffer(GLfloat* data, GLsizei count, GLuint componentCount);
			~Buffer();

			void bind() const;
			void unbind() const;

			inline GLuint getComponentCount() const { return _componentCount; }
		};
	}
}