#pragma once

#include "../maths/maths.h"
#include "renderer2d.h"
#include "shader.h"
#include "buffers/buffer.h"
#include "buffers/indexbuffer.h"
#include "buffers/vertexarray.h"

namespace pixel {
	namespace graphics {

		struct VertexData {
			maths::vec3f vertex;
			maths::vec2f textureCoord;
			unsigned int colour;
		};

		class Renderable2D {
		private:
			void setDefaultTextureCoord() {
				_textureCoord.push_back(maths::vec2f(0, 0));
				_textureCoord.push_back(maths::vec2f(0, 1));
				_textureCoord.push_back(maths::vec2f(1, 1));
				_textureCoord.push_back(maths::vec2f(1, 0));
			}

		protected:
			maths::vec3f _position;
			maths::vec2f _size;
			maths::vec4f _colour;
			std::vector<maths::vec2f> _textureCoord;

			Renderable2D() {
				setDefaultTextureCoord();
			}

		public:
			Renderable2D(maths::vec3f position, maths::vec2f size, maths::vec4f colour) : _position(position), _size(size), _colour(colour) {
				setDefaultTextureCoord();
			}
			virtual ~Renderable2D() { }

			virtual void submit(Renderer2D* renderer) const {
				renderer->submit(this);
			}

			inline const maths::vec3f& getPosition() const { return _position; }
			inline const maths::vec2f& getSize() const { return _size; }
			inline const maths::vec4f& getColour() const { return _colour; }
			inline const std::vector<maths::vec2f>& getTextureCoord() const { return _textureCoord; }
		};
	}
}