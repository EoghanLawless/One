#pragma once

#include <cstddef>

#include "graphics/renderers/renderer.h"
#include "graphics/renderables/renderable.h"

#include "graphics/buffers/indexbuffer.h"

namespace pixel {
	namespace graphics {

#define RENDERER_VERTEX_SIZE	sizeof(VertexData)
#define RENDERER_SPRITE_SIZE	RENDERER_VERTEX_SIZE * 4
#define RENDERER_BUFFER_SIZE	RENDERER_SPRITE_SIZE * RENDERER_MAX_SPRITES
#define RENDERER_INDICES_SIZE	RENDERER_MAX_SPRITES * 6
#define RENDERER_MAX_SPRITES	60000
#define RENDERER_MAX_TEXTURES	32

#define SHADER_VERTEX_INDEX		0
#define SHADER_TEXTURE_COORD_INDEX		1
#define SHADER_TEXTURE_ID_INDEX		2
#define SHADER_COLOUR_INDEX		3

		class BatchRenderer : public Renderer {
		private:
			GLuint _vertexArray;
			GLuint _buffer;
			IndexBuffer* _indexBuffer;
			GLsizei _indexCount;

			std::vector<GLuint> _textures;

			VertexData* _dataBuffer;

			void init();

		public:
			BatchRenderer();
			~BatchRenderer();

			void begin() override;
			void submit(const Renderable* renderable) override;
			void drawString(const std::string& text, const maths::vec3f& position, const Font& font, const unsigned int colour) override;
			void end() override;
			void flush() override;
		};

	}
}