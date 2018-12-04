#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace pixel {
	namespace graphics {

#define NUM_KEYS 104
#define NUM_BUTTONS 16

		static void windowResizeCallback(GLFWwindow*, int, int);

		static void keyCallback(GLFWwindow*, int, int, int, int);
		static void mouseButtonCallback(GLFWwindow*, int, int, int);
		static void mousePositionCallback(GLFWwindow*, double, double);

		class Window {
		private:
			GLFWwindow* _window;
			const char* _title;
			int _width, _height;

			bool init();

		public:
			Window(const char *title, int width, int height);
			~Window();

			bool keys[NUM_KEYS];
			bool mouseButtons[NUM_BUTTONS];
			double mouseX, mouseY;

			void update();
			void clear() const;
			bool closed() const;

			inline int getWidth() const { return _width;  }
			inline int getHeight() const { return _height;  }

			inline bool keyPressed(unsigned int key) { return (key < 0 || key > NUM_KEYS) ? false : keys[key]; }
			inline bool mousePressed(unsigned int button) { return (button < 0 || button > NUM_BUTTONS) ? false : mouseButtons[button]; }
			void getMousePos(double& xpos, double& ypos);
		};
	}
}