#include "indexbuffer.h"

namespace pixel {
	namespace graphics {

		IndexBuffer::IndexBuffer(GLushort* data, GLsizei count) : _count(count) {
			glGenBuffers(1, &_indexBufferId);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBufferId);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLushort), data, GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}

		IndexBuffer::~IndexBuffer() {
			glDeleteBuffers(1, &_indexBufferId);
		}

		void IndexBuffer::bind() const {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBufferId);
		}
		void IndexBuffer::unbind() const {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}
	}
}