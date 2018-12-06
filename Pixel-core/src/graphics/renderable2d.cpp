#include "renderable2d.h"

namespace pixel {
	namespace graphics {

		Renderable2D::Renderable2D(maths::vec3f position, maths::vec2f size, maths::vec4f colour, Shader& shader) : _position(position), _size(size), _colour(colour), _shader(shader) {
			_vertexArray = new VertexArray();

			GLfloat vertices[] = {
				0,		0,		0,
				0,		size.y,	0,
				size.x, size.y, 0,
				size.x, 0,		0,
			};

			GLfloat colours[] = {
				colour.x, colour.y, colour.z, colour.w,
				colour.x, colour.y, colour.z, colour.w,
				colour.x, colour.y, colour.z, colour.w,
				colour.x, colour.y, colour.z, colour.w
			};

			std::cout << position << std::endl;
			std::cout << size << std::endl;
			std::cout << colour << std::endl;

			_vertexArray->addBuffer(new Buffer(vertices, 4 * 3, 3), 0);
			_vertexArray->addBuffer(new Buffer(colours, 4 * 4, 4), 1);

			GLushort indices[] = { 0,1,2,2,3,0 };
			_indexBuffer = new IndexBuffer(indices, 6);
		}

		Renderable2D::~Renderable2D() {
			delete _vertexArray;
			delete _indexBuffer;
		}
	}
}