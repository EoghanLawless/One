#include "src/graphics/window.h"
#include "src/graphics/shader.h"
#include "src/graphics/buffers/buffer.h"
#include "src/graphics/buffers/indexbuffer.h"
#include "src/graphics/buffers/vertexarray.h"
#include "src/maths/mat4.h"

int main() {

	using namespace pixel;
	using namespace graphics;
	using namespace maths;

	Window window("Pixel", 1600, 900);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	GLfloat vertices[] = {
		0, 0, 0,
		0, 7, 0,
		4, 7, 0,
		4, 0, 0
	};

	GLfloat coloursA[] = {
		0.5, 0.1, 0.6, 1.0,
		0.5, 0.1, 0.6, 1.0,
		0.5, 0.1, 0.6, 1.0,
		0.5, 0.1, 0.6, 1.0
	};
	
	GLfloat coloursB[] = {
		0.1, 0.6, 0.3, 1.0,
		0.1, 0.6, 0.3, 1.0,
		0.1, 0.6, 0.3, 1.0,
		0.1, 0.6, 0.3, 1.0
	};

	GLushort indices[] = {
		0,1,2,
		2,3,0
	};

	VertexArray sprite1, sprite2;
	IndexBuffer indexBufferObject(indices, 6);

	sprite1.addBuffer(new Buffer(vertices, 4 * 3, 3), 0);
	sprite1.addBuffer(new Buffer(coloursA, 4 * 4, 4), 1);

	sprite2.addBuffer(new Buffer(vertices, 4 * 3, 3), 0);
	sprite2.addBuffer(new Buffer(coloursB, 4 * 4, 4), 1);

	Shader shader("src/shaders/basic.vert", "src/shaders/basic.frag");
	shader.enable();

	mat4 ortho = mat4::orthographic(0.0f, 16.0f, 0.0f, 9.0f, -1.0f, 1.0f);
	shader.setUniformMat4("pr_matrix", ortho);

	shader.setUniform2f("light_pos", vec2d(0.0f, 0.0f));

	double mx, my;

	while(!window.closed()) {
		window.clear();

		window.getMousePos(mx, my);

		shader.setUniform2f("light_pos", vec2d((float)(mx * 16.0f / 1600.0f), (float)(9.0f - my * 9.0f / 900.0f)));

		sprite1.bind();
		indexBufferObject.bind();

		shader.setUniformMat4("ml_matrix", mat4::translation(vec3d(3.0f, 1.0f, 0.0f)));
		glDrawElements(GL_TRIANGLES, indexBufferObject.getComponentCount(), GL_UNSIGNED_SHORT, 0);

		indexBufferObject.unbind();
		sprite1.unbind();


		sprite2.bind();
		indexBufferObject.bind();

		shader.setUniformMat4("ml_matrix", mat4::translation(vec3d(9.0f, 1.0f, 0.0f)));
		glDrawElements(GL_TRIANGLES, indexBufferObject.getComponentCount(), GL_UNSIGNED_SHORT, 0);

		indexBufferObject.unbind();
		sprite2.unbind();

		window.update();
	}

	return 0;
}