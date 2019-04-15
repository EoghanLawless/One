#pragma once

#include <iostream>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "fontmanager.h"

namespace pixel {
	namespace graphics {

#define NUM_KEYS 104
#define NUM_BUTTONS 16

		class Window {
		private:
			GLFWwindow* _window;
			bool _closed;

			const char* _title;
			int _width, _height;

			bool _keys[NUM_KEYS];
			bool _keyState[NUM_KEYS];
			bool _keyTyped[NUM_KEYS];
			bool _mouseButtons[NUM_BUTTONS];
			bool _mouseState[NUM_BUTTONS];
			bool _mouseClicked[NUM_BUTTONS];
			double _mx, _my;

			bool init();

			friend static void windowResizeCallback(GLFWwindow*, int, int);
			friend static void keyCallback(GLFWwindow*, int, int, int, int);
			friend static void mouseButtonCallback(GLFWwindow*, int, int, int);
			friend static void mousePositionCallback(GLFWwindow*, double, double);

		public:
			Window(const char *title, int width, int height);
			~Window();

			void update();
			void clear() const;
			bool closed() const;

			inline int getWidth() const { return _width; }
			inline int getHeight() const { return _height; }

			bool keyPressed(unsigned int key) const;
			bool keyTyped(unsigned int key) const;
			bool mousePressed(unsigned int button) const;
			bool mouseClicked(unsigned int button) const;
			void getMousePos(double& xpos, double& ypos);
		};

	}
}