#include "window.h"

namespace pixel {
	namespace graphics {

		void windowResizeCallback(GLFWwindow*, int, int);
		void keyCallback(GLFWwindow*, int, int, int, int);
		void mouseButtonCallback(GLFWwindow*, int, int, int);
		void mousePositionCallback(GLFWwindow*, double, double);

		Window::Window(const char *title, int width, int height) {
			_title = title;
			_width = width;
			_height = height;

			if (!init()) 
				glfwTerminate();

			for (int key = 0; key < NUM_KEYS; key++) {
				_keys[key] = false;
				_keyState[key] = false;
				_keyTyped[key] = false;
			}

			for (int button = 0; button < NUM_BUTTONS; button++) {
				_mouseButtons[button] = false;
				_mouseState[button] = false;
				_mouseClicked[button] = false;
			}

			FontManager::add(new Font("Courier New", "Courier.ttf", 16));
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
			glfwSetFramebufferSizeCallback(_window, windowResizeCallback);
			glfwSetKeyCallback(_window, keyCallback);
			glfwSetMouseButtonCallback(_window, mouseButtonCallback);
			glfwSetCursorPosCallback(_window, mousePositionCallback);
			glfwSwapInterval(0);

			if(glewInit() != GLEW_OK) {
				std::cout << "Failed to initialise GLEW" << std::endl;
				return false;
			}

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			std::cout << "OpenGL " << glGetString(GL_VERSION) << std::endl;

			return true;
		}
		

		void Window::update() {
			for (int key = 0; key < NUM_KEYS; key++)
				_keyTyped[key] = _keys[key] && !_keyState[key];
			memcpy(_keyState, _keys, NUM_KEYS);

			for (int button = 0; button < NUM_BUTTONS; button++)
				_mouseClicked[button] = _mouseButtons[button] && !_mouseState[button];
			memcpy(_mouseState, _mouseButtons, NUM_BUTTONS);

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



		bool Window::keyPressed(unsigned int key) const {
			if (key >= NUM_KEYS)
				return false;

			return _keys[key];
		}
		bool Window::keyTyped(unsigned int key) const {
			if (key >= NUM_KEYS)
				return false;

			return _keyTyped[key];

		}
		bool Window::mousePressed(unsigned int button) const {
			if (button >= NUM_BUTTONS)
				return false;

			return _mouseButtons[button];
		}
		bool Window::mouseClicked(unsigned int button) const {
			if (button >= NUM_BUTTONS)
				return false;

			return _mouseClicked[button];
		}
		void Window::getMousePos(double& x, double& y) {
			x = _mx;
			y = _my;
		}


		void windowResizeCallback(GLFWwindow *window, int width, int height) {
			glViewport(0, 0, width, height);
			
			Window* win = (Window*)glfwGetWindowUserPointer(window);
			win->_width = width;
			win->_height = height;
		}
		void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
			Window* win = (Window*) glfwGetWindowUserPointer(window);
			win->_keys[key] = (action != GLFW_RELEASE);
		}
		void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
			Window* win = (Window*) glfwGetWindowUserPointer(window);
			win->_mouseButtons[button] = (action != GLFW_RELEASE);
		}
		void mousePositionCallback(GLFWwindow* window, double x, double y) {
			Window* win = (Window*)glfwGetWindowUserPointer(window);
			win->_mx = x;
			win->_my = y;
		}

	}
}