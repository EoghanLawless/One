#include "buffer.h"

namespace pixel {
	namespace graphics {

		Buffer::Buffer(GLfloat* data, GLsizei count, GLuint componentCount) : _componentCount(componentCount) {
			glGenBuffers(1, &_bufferId);
			glBindBuffer(GL_ARRAY_BUFFER, _bufferId);
			glBufferData(GL_ARRAY_BUFFER, count * sizeof(GLfloat), data, GL_STATIC_DRAW);
		}
		Buffer::~Buffer() {
			glDeleteBuffers(1, &_bufferId);
		}

		void Buffer::bind() const {
			glBindBuffer(GL_ARRAY_BUFFER, _bufferId);
		}
		void Buffer::unbind() const {
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
	}
}