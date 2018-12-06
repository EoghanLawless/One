#pragma once

#include <GL/glew.h>

namespace pixel {
	namespace graphics {

		class IndexBuffer {
		private:
			GLuint _bufferId;
			GLuint _count;

		public:
			IndexBuffer(GLushort* data, GLsizei count);

			void bind() const;
			void unbind() const;

			inline GLuint getComponentCount() const { return _count; }
		};
	}
}