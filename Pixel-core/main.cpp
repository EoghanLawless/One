#include "src/graphics/window.h"
#include "src/maths/vec3d.h"
#include <GLFW/glfw3.h>
#include <iostream>

int main() {

	using namespace pixel;
	using namespace graphics;
	using namespace maths;

	Window window("Pixel", 640, 480);
	glClearColor(1.0f, 0.0f, 1.0f, 1.0f);

	while(!window.closed()) {
		double x, y;
		window.getMousePos(x, y);
		vec3d a(1, 2, 3);
		vec3d b(1, 3, 2);
		if (window.mousePressed(GLFW_MOUSE_BUTTON_LEFT)) {
			std::cout << a << ", " << (a + b) << std::endl;
		}

		window.clear();
		glBegin(GL_QUADS);
		glVertex2f(-0.5f, -0.5f);
		glVertex2f(0.5f, -0.5f);
		glVertex2f(0.5f, 0.5f);
		glVertex2f(-0.5f, 0.5f);
		glEnd();
		window.update();
	}

	return 0;
}