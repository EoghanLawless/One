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

			for (int key = 0; key < NUM_KEYS; key++) {
				keys[key] = false;
			}

			for (int button = 0; button < NUM_BUTTONS; button++) {
				mouseButtons[button] = false;
			}
		}

		Window::~Window() {
			glfwTerminate();
		}

		bool Window::init() {
			if(!glfwInit()) {
				std::cout << "Failed to initialise GLFW" << std::endl;
				return false;
			}

			_window = glfwCreateWindow(_width, _height, _title, NULL, NULL);

			if(!_window) {
				std::cout << "Failed to create GLFW window" << std::endl;
				return false;
			}

			glfwMakeContextCurrent(_window);
			glfwSetWindowUserPointer(_window, this);
			glfwSetWindowSizeCallback(_window, windowResizeCallback);
			glfwSetKeyCallback(_window, keyCallback);
			glfwSetMouseButtonCallback(_window, mouseButtonCallback);
			glfwSetCursorPosCallback(_window, mousePositionCallback);

			if(glewInit() != GLEW_OK) {
				std::cout << "Failed to initialise GLEW" << std::endl;
				return false;
			}

			std::cout << "OpenGL " << glGetString(GL_VERSION) << std::endl;

			return true;
		}
		
		void Window::update() {

			GLenum error = glGetError();

			if (error != GL_NO_ERROR) {
				std::cout << "OpenGL error: " << error << std::endl;
			}

			glfwPollEvents();
			glfwSwapBuffers(_window);
		}

		void Window::clear() const {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		bool Window::closed() const {
			return glfwWindowShouldClose(_window);
		}

		void windowResizeCallback(GLFWwindow *window, int width, int height) {
			glViewport(0, 0, width, height);
		}

		void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
			Window* win = (Window*) glfwGetWindowUserPointer(window);
			win->keys[key] = (action != GLFW_RELEASE);
		}

		void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
			Window* win = (Window*) glfwGetWindowUserPointer(window);
			win->mouseButtons[button] = (action != GLFW_RELEASE);
		}

		void mousePositionCallback(GLFWwindow* window, double xpos, double ypos) {
			Window* win = (Window*)glfwGetWindowUserPointer(window);
			win->mouseX = xpos;
			win->mouseY = ypos;
		}

		void Window::getMousePos(double& xpos, double& ypos) {
			xpos = mouseX;
			ypos = mouseY;
		}
	}
}