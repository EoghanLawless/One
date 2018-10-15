#include "window.h"
#include <iostream>

namespace pixel {
	namespace graphics {
		Window::Window(const char *title, int width, int height) {
			_title = title;
			_width = width;
			_height = height;

			if (!init()) {
				glfwTerminate();
			}
		}

		Window::~Window() {
			glfwTerminate();
		}

		bool Window::init() {
			if (!glfwInit()) {
				std::cout << "Failed to initialise GLFW" << std::endl;
				return false;
			}

			_window = glfwCreateWindow(_width, _height, _title, NULL, NULL);

			if (!_window) {
				std::cout << "Failed to create GLFW window" << std::endl;
				return false;
			}

			glfwMakeContextCurrent(_window);

			glfwSetWindowSizeCallback(_window, windowResize);

			return true;
		}

		void Window::update() {
			glfwPollEvents();
			glfwSwapBuffers(_window);
		}

		void Window::clear() const {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		bool Window::closed() const {
			return glfwWindowShouldClose(_window);
		}

		void windowResize(GLFWwindow *window, int width, int height) {
			glViewport(0, 0, width, height);
		}
	}
}