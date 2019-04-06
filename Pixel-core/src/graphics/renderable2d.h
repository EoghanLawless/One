#pragma once

#include "../maths/maths.h"
#include "shader.h"
#include "buffers/buffer.h"
#include "buffers/indexbuffer.h"
#include "buffers/vertexarray.h"

namespace pixel {
	namespace graphics {

		struct VertexData {
			maths::vec3f vertex;
			unsigned int colour;
		};

		class Renderable2D {
		protected:
			maths::vec3f _position;
			maths::vec2f _size;
			maths::vec4f _colour;

		public:
			Renderable2D(maths::vec3f position, maths::vec2f size, maths::vec4f colour) : _position(position), _size(size), _colour(colour) { }
			virtual ~Renderable2D() { }

			inline const maths::vec3f& getPosition() const { return _position; }
			inline const maths::vec2f& getSize() const { return _size; }
			inline const maths::vec4f& getColour() const { return _colour; }
		};
	}
}