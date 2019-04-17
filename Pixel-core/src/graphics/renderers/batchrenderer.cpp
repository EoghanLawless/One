#include "batchrenderer.h"

namespace pixel {
	namespace graphics {

		BatchRenderer::BatchRenderer() {
			init();
		}
		BatchRenderer::~BatchRenderer() {
			delete _indexBuffer;
			glDeleteBuffers(1, &_buffer);
		}

		void BatchRenderer::init() {
			glGenVertexArrays(1, &_vertexArray);
			glGenBuffers(1, &_buffer);

			glBindVertexArray(_vertexArray);
			glBindBuffer(GL_ARRAY_BUFFER, _buffer);
			glBufferData(GL_ARRAY_BUFFER, RENDERER_BUFFER_SIZE, NULL, GL_DYNAMIC_DRAW);

			glEnableVertexAttribArray(SHADER_VERTEX_INDEX);
			glEnableVertexAttribArray(SHADER_TEXTURE_COORD_INDEX);
			glEnableVertexAttribArray(SHADER_TEXTURE_ID_INDEX);
			glEnableVertexAttribArray(SHADER_COLOUR_INDEX);

			glVertexAttribPointer(SHADER_VERTEX_INDEX,			3, GL_FLOAT,			GL_FALSE,	RENDERER_VERTEX_SIZE, (const GLvoid*) 0);
			glVertexAttribPointer(SHADER_TEXTURE_COORD_INDEX,	2, GL_FLOAT,			GL_FALSE,	RENDERER_VERTEX_SIZE, (const GLvoid*) (offsetof(VertexData, VertexData::textureCoord)));
			glVertexAttribPointer(SHADER_TEXTURE_ID_INDEX,		1, GL_FLOAT,			GL_FALSE,	RENDERER_VERTEX_SIZE, (const GLvoid*) (offsetof(VertexData, VertexData::textureId)));
			glVertexAttribPointer(SHADER_COLOUR_INDEX,			4, GL_UNSIGNED_BYTE,	GL_TRUE,	RENDERER_VERTEX_SIZE, (const GLvoid*) (offsetof(VertexData, VertexData::colour)));

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

		void BatchRenderer::begin() {
			glBindBuffer(GL_ARRAY_BUFFER, _buffer);
			_dataBuffer = (VertexData*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		}

		void BatchRenderer::submit(const Renderable* renderable) {
			const maths::vec3f& position = renderable->getPosition();
			const maths::vec2f& size = renderable->getSize();
			const unsigned int colour = renderable->getColour();
			const std::vector<maths::vec2f>& textureCoord = renderable->getTextureCoord();
			const GLuint textureId = renderable->getTextureId();

			float samplerIndex = 0.0f;

			if (textureId > 0) {

				bool exists = false;

				for (int index = 0; index < _textures.size(); index++) {
					if (_textures[index] == textureId) {
						samplerIndex = (float)(index + 1);
						exists = true;
						break;
					}
				}

				if (!exists) {
					if (_textures.size() >= RENDERER_MAX_TEXTURES) {
						end();
						flush();
						begin();
					}

					_textures.push_back(textureId);
					samplerIndex = (float)(_textures.size() - 1);
				}
				
			}


			_dataBuffer->vertex = *_currentTransformation * position;
			_dataBuffer->textureCoord = textureCoord[0];
			_dataBuffer->textureId = samplerIndex;
			_dataBuffer->colour = colour;
			_dataBuffer++;

			_dataBuffer->vertex = *_currentTransformation * maths::vec3f(position.x, position.y + size.y, position.z);
			_dataBuffer->textureCoord = textureCoord[1];
			_dataBuffer->textureId = samplerIndex;
			_dataBuffer->colour = colour;
			_dataBuffer++;

			_dataBuffer->vertex = *_currentTransformation * maths::vec3f(position.x + size.x, position.y + size.y, position.z);
			_dataBuffer->textureCoord = textureCoord[2];
			_dataBuffer->textureId = samplerIndex;
			_dataBuffer->colour = colour;
			_dataBuffer++;

			_dataBuffer->vertex = *_currentTransformation * maths::vec3f(position.x + size.x, position.y, position.z);
			_dataBuffer->textureCoord = textureCoord[3];
			_dataBuffer->textureId = samplerIndex;
			_dataBuffer->colour = colour;
			_dataBuffer++;

			_indexCount += 6;
		}

		void BatchRenderer::drawString(const std::string& text, const maths::vec3f& position, const Font& font, const unsigned int colour) {

			float samplerIndex = 0.0f;
			bool exists = false;

			for (int index = 0; index < _textures.size(); index++) {
				if (_textures[index] == font.getId()) {
					samplerIndex = (float)(index + 1);
					exists = true;
					break;
				}
			}

			if (!exists) {
				if (_textures.size() >= RENDERER_MAX_TEXTURES) {
					end();
					flush();
					begin();
				}

				_textures.push_back(font.getId());
				samplerIndex = (float)_textures.size();
			}

			const maths::vec2f& scale = font.getScale();

			float x = position.x;

			for (int i = 0; i < text.length(); i++) {
				char ch = text.at(i);
				texture_glyph_t* glyph = texture_font_get_glyph(font.getFont(), ch);

				if (glyph != NULL) {

					if (i > 0) {
						float kerning = texture_glyph_get_kerning(glyph, text.at(i - 1));
						x += kerning / scale.x;
					}

					float x0 = x + glyph->offset_x / scale.x;
					float y0 = position.y + glyph->offset_y / scale.y;
					float x1 = x0 + glyph->width / scale.x;
					float y1 = y0 - glyph->height / scale.y;

					float s0 = glyph->s0;
					float t0 = glyph->t0;
					float s1 = glyph->s1;
					float t1 = glyph->t1;

					_dataBuffer->vertex = *_currentTransformation * maths::vec3f(x0, y0, 0);
					_dataBuffer->textureCoord = maths::vec2f(s0, t0);
					_dataBuffer->textureId = samplerIndex;
					_dataBuffer->colour = colour;
					_dataBuffer++;

					_dataBuffer->vertex = *_currentTransformation * maths::vec3f(x0, y1, 0);
					_dataBuffer->textureCoord = maths::vec2f(s0, t1);
					_dataBuffer->textureId = samplerIndex;
					_dataBuffer->colour = colour;
					_dataBuffer++;

					_dataBuffer->vertex = *_currentTransformation * maths::vec3f(x1, y1, 0);
					_dataBuffer->textureCoord = maths::vec2f(s1, t1);
					_dataBuffer->textureId = samplerIndex;
					_dataBuffer->colour = colour;
					_dataBuffer++;

					_dataBuffer->vertex = *_currentTransformation * maths::vec3f(x1, y0, 0);
					_dataBuffer->textureCoord = maths::vec2f(s1, t0);
					_dataBuffer->textureId = samplerIndex;
					_dataBuffer->colour = colour;
					_dataBuffer++;

					_indexCount += 6;

					x += glyph->advance_x / scale.x;
				}
			}
		}


		void BatchRenderer::end() {
			glUnmapBuffer(GL_ARRAY_BUFFER);
			glBindBuffer(GL_ARRAY_BUFFER, _buffer);
		}

		void BatchRenderer::flush() {

			for (int i = 0; i < _textures.size(); i++) {
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, _textures[i]);
			}

			glBindVertexArray(_vertexArray);
			_indexBuffer->bind();

			glDrawElements(GL_TRIANGLES, _indexCount, GL_UNSIGNED_INT, NULL);

			_indexBuffer->unbind();
			glBindVertexArray(0);

			_indexCount = 0;
			_textures.clear();
		}

	}
}