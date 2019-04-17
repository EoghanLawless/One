#pragma once

#include "graphics/renderers/renderer.h"
#include "graphics/texture.h"
#include "graphics/shader.h"

#include "graphics/buffers/buffer.h"
#include "graphics/buffers/indexbuffer.h"
#include "graphics/buffers/vertexarray.h"

#include "maths/maths.h"

namespace pixel {
	namespace graphics {

		struct VertexData {
			maths::vec3f vertex;
			maths::vec2f textureCoord;
			float textureId;
			unsigned int colour;
		};

		class Renderable {
		private:
			void setDefaultTextureCoord() {
				_textureCoord.push_back(maths::vec2f(0, 0));
				_textureCoord.push_back(maths::vec2f(0, 1));
				_textureCoord.push_back(maths::vec2f(1, 1));
				_textureCoord.push_back(maths::vec2f(1, 0));
			}

		protected:
			std::vector<maths::vec2f> _textureCoord;
			Texture* _texture;

			Renderable() : _texture(nullptr) {
				setDefaultTextureCoord();
			}

		public:
			maths::vec3f position;
			maths::vec2f size;
			unsigned int colour;

			Renderable(maths::vec3f p, maths::vec2f s, unsigned int c) : position(p), size(s), colour(c), _texture(nullptr) {
				setDefaultTextureCoord();
			}
			virtual ~Renderable() { }

			virtual void submit(Renderer* renderer) const {
				renderer->submit(this);
			}

			void setColour(unsigned int colour) { colour = colour; }
			void setColour(const maths::vec4f& c) {
				int r = c.x * 255.0f;
				int g = c.y * 255.0f;
				int b = c.z * 255.0f;
				int a = c.w * 255.0f;

				colour = a << 24 | b << 16 | g << 8 | r;
			}

			inline const std::vector<maths::vec2f>& getTextureCoord() const { return _textureCoord; }
			inline const GLuint& getTextureId() const { return _texture == nullptr ? 0 : _texture->getId(); }
		};

	}
}