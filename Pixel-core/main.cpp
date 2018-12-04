#include "src/graphics/window.h"
#include <GLFW/glfw3.h>
#include <iostream>

int main() {

	using namespace pixel;
	using namespace graphics;

	Window window("Pixel", 640, 480);
	glClearColor(1.0f, 0.0f, 1.0f, 1.0f);

	while(!window.closed()) {
		double x, y;
		window.getMousePos(x, y);
		if (window.mousePressed(GLFW_MOUSE_BUTTON_LEFT)) {
			std::cout << x << ", " << y << std::endl;
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