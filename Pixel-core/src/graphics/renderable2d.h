#pragma once

#include "buffers/buffer.h"
#include "buffers/indexbuffer.h"
#include "buffers/vertexarray.h"
#include "shader.h"
#include "../maths/maths.h"

namespace pixel {
	namespace graphics {

		struct VertexData {
			maths::vec3f vertex;
			maths::vec4f colour;
		};

		class Renderable2D {
		protected:
			VertexArray* _vertexArray;
			IndexBuffer* _indexBuffer;
			Shader& _shader;

			maths::vec3f _position;
			maths::vec2f _size;
			maths::vec4f _colour;

		public:
			Renderable2D() = default;
			Renderable2D(maths::vec3f position, maths::vec2f size, maths::vec4f colour, Shader& shader);
			virtual ~Renderable2D();

			inline const VertexArray* getVAO() const { return _vertexArray; }
			inline const IndexBuffer* getIBO() const { return _indexBuffer; }
			inline Shader& getShader() const { return _shader; }

			inline const maths::vec3f& getPosition() const { return _position; }
			inline const maths::vec2f& getSize() const { return _size; }
			inline const maths::vec4f& getColour() const { return _colour; }
		};
	}
}