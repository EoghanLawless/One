#include "staticsprite.h"

namespace pixel {
	namespace graphics {

		StaticSprite::StaticSprite(float x, float y, float w, float h, const maths::vec4f& colour, Shader& shader) : Renderable2D(maths::vec3f(x,y,0), maths::vec2f(w,h), colour), _shader(shader) {
			_vertexArray = new VertexArray();

			GLfloat vertices[] = {
				0, 0, 0,
				0, h, 0,
				w, h, 0,
				w, 0, 0,
			};

			GLfloat colours[] = {
				colour.x, colour.y, colour.z, colour.w,
				colour.x, colour.y, colour.z, colour.w,
				colour.x, colour.y, colour.z, colour.w,
				colour.x, colour.y, colour.z, colour.w
			};

			_vertexArray->addBuffer(new Buffer(vertices, 4 * 3, 3), 0);
			_vertexArray->addBuffer(new Buffer(colours, 4 * 4, 4), 1);

			GLushort indices[] = { 0,1,2,2,3,0 };
			_indexBuffer = new IndexBuffer(indices, 6);
		}
		StaticSprite::~StaticSprite() {
			delete _vertexArray;
			delete _indexBuffer;
		}
	}
}