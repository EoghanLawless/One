#include "batchrenderer2d.h"

namespace pixel {
	namespace graphics {

		BatchRenderer2D::BatchRenderer2D() {
			init();
		}
		BatchRenderer2D::~BatchRenderer2D() {
			delete _indexBuffer;
			glDeleteBuffers(1, &_buffer);
		}

		void BatchRenderer2D::init() {
			glGenVertexArrays(1, &_vertexArray);
			glGenBuffers(1, &_buffer);

			glBindVertexArray(_vertexArray);
			glBindBuffer(GL_ARRAY_BUFFER, _buffer);
			glBufferData(GL_ARRAY_BUFFER, RENDERER_BUFFER_SIZE, NULL, GL_DYNAMIC_DRAW);

			glEnableVertexAttribArray(SHADER_VERTEX_INDEX);
			glEnableVertexAttribArray(SHADER_TEXTURE_COORD_INDEX);
			glEnableVertexAttribArray(SHADER_COLOUR_INDEX);

			glVertexAttribPointer(SHADER_VERTEX_INDEX, 3, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, (const GLvoid*) 0);
			glVertexAttribPointer(SHADER_TEXTURE_COORD_INDEX, 2, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, (const GLvoid*) (offsetof(VertexData, VertexData::textureCoord)));
			glVertexAttribPointer(SHADER_COLOUR_INDEX, 4, GL_UNSIGNED_BYTE, GL_TRUE, RENDERER_VERTEX_SIZE, (const GLvoid*) (offsetof(VertexData, VertexData::colour)));

			glBindBuffer(GL_ARRAY_BUFFER, 0);

			GLuint* indices = new GLuint[RENDERER_INDICES_SIZE];

			int offset = 0;
			for (int index = 0; index < RENDERER_INDICES_SIZE; index += 6) {
				indices[index + 0] = offset + 0;
				indices[index + 1] = offset + 1;
				indices[index + 2] = offset + 2;

				indices[index + 3] = offset + 2;
				indices[index + 4] = offset + 3;
				indices[index + 5] = offset + 0;

				offset += 4;
			}

			_indexBuffer = new IndexBuffer(indices, RENDERER_INDICES_SIZE);

			glBindVertexArray(0);
		}

		void BatchRenderer2D::begin() {
			glBindBuffer(GL_ARRAY_BUFFER, _buffer);
			_dataBuffer = (VertexData*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		}

		void BatchRenderer2D::submit(const Renderable2D* renderable) {
			const maths::vec3f& position = renderable->getPosition();
			const maths::vec2f& size = renderable->getSize();
			const maths::vec4f& colour = renderable->getColour();
			const std::vector<maths::vec2f>& textureCoord = renderable->getTextureCoord();

			int r = colour.x * 255.0f;
			int g = colour.y * 255.0f;
			int b = colour.z * 255.0f;
			int a = colour.w * 255.0f;

			unsigned int c = a << 24 | b << 16 | g << 8 | r;

			_dataBuffer->vertex = *_currentTransformation * position;
			_dataBuffer->textureCoord = textureCoord[0];
			_dataBuffer->colour = c;
			_dataBuffer++;

			_dataBuffer->vertex = *_currentTransformation * maths::vec3f(position.x, position.y + size.y, position.z);
			_dataBuffer->textureCoord = textureCoord[1];
			_dataBuffer->colour = c;
			_dataBuffer++;

			_dataBuffer->vertex = *_currentTransformation * maths::vec3f(position.x + size.x, position.y + size.y, position.z);
			_dataBuffer->textureCoord = textureCoord[2];
			_dataBuffer->colour = c;
			_dataBuffer++;

			_dataBuffer->vertex = *_currentTransformation * maths::vec3f(position.x + size.x, position.y, position.z);
			_dataBuffer->textureCoord = textureCoord[3];
			_dataBuffer->colour = c;
			_dataBuffer++;

			_indexCount += 6;
		}

		void BatchRenderer2D::end() {
			glUnmapBuffer(GL_ARRAY_BUFFER);
			glBindBuffer(GL_ARRAY_BUFFER, _buffer);
		}

		void BatchRenderer2D::flush() {
			glBindVertexArray(_vertexArray);
			_indexBuffer->bind();

			glDrawElements(GL_TRIANGLES, _indexCount, GL_UNSIGNED_INT, NULL);

			_indexBuffer->unbind();
			glBindVertexArray(0);

			_indexCount = 0;
		}
	}
}