#pragma once

#include <GL/glew.h>

namespace pixel {
	namespace graphics {

		class IndexBuffer {
		private:
			GLuint _indexBufferId;
			GLuint _count;

		public:
			IndexBuffer(GLushort* data, GLsizei count);
			IndexBuffer(GLuint* data, GLsizei count);
			~IndexBuffer();

			void bind() const;
			void unbind() const;

			inline GLuint getCount() const { return _count; }
		};
	}
}