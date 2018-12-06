#pragma once

#include "renderable2d.h"

namespace pixel {
	namespace graphics {
		class StaticSprite : public Renderable2D {
		private:
			maths::vec3f _position;
			maths::vec2f _size;
			maths::vec4f _colour;

			Shader& _shader;
			VertexArray* _vertexArray;
			IndexBuffer* _indexBuffer;

		public:
			StaticSprite(float x, float y, float w, float h, const maths::vec4f& colour, Shader& shader);
			~StaticSprite();

			inline Shader& getShader() const { return _shader; }
			inline const VertexArray* getVertexArray() const { return _vertexArray; }
			inline const IndexBuffer* getIndexBuffer() const { return _indexBuffer; }
		};
	}
}