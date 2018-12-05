#include "src/graphics/window.h"
#include "src/graphics/shader.h"
#include "src/maths/mat4.h"

int main() {

	using namespace pixel;
	using namespace graphics;
	using namespace maths;

	Window window("Pixel", 640, 480);
	glClearColor(1.0f, 0.0f, 1.0f, 1.0f);

	GLfloat vertices[] = {
		1, 1, 0,
		1, 2, 0,
		2, 2, 0,
		2, 2, 0,
		2, 1, 0,
		1, 1, 0,
	};

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	Shader shader("src/shaders/basic.vert", "src/shaders/basic.frag");
	shader.enable();

	mat4 ortho = mat4::orthographic(0.0f, 16.0f, 0.0f, 9.0f, -1.0f, 1.0f);
	shader.setUniformMat4("pr_matrix", ortho);

	while(!window.closed()) {
		window.clear();
		glDrawArrays(GL_TRIANGLES, 0, 6);
		window.update();
	}

	return 0;
}