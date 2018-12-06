#include "indexbuffer.h"

namespace pixel {
	namespace graphics {

		IndexBuffer::IndexBuffer(GLushort* data, GLsizei count) : _count(count) {
			glGenBuffers(1, &_bufferId);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _bufferId);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLushort), data, GL_STATIC_DRAW);
		}

		void IndexBuffer::bind() const {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _bufferId);
		}
		void IndexBuffer::unbind() const {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}
	}
}