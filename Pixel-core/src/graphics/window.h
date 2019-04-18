#pragma once

#include <Windows.h>
#include <iostream>

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"

#include "fontmanager.h"

namespace pixel {
	namespace graphics {

#define NUM_KEYS 348
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

			friend void windowResizeCallback(GLFWwindow*, int, int);
			friend void keyCallback(GLFWwindow*, int, int, int, int);
			friend void mouseButtonCallback(GLFWwindow*, int, int, int);
			friend void mousePositionCallback(GLFWwindow*, double, double);

		public:
			Window(const char*, int, int);
			~Window();

			void update();
			void clear() const;
			bool closed() const;

			inline int getWidth() const { return _width; }
			inline int getHeight() const { return _height; }

			bool keyPressed(unsigned int) const;
			bool keyTyped(unsigned int) const;
			bool mousePressed(unsigned int) const;
			bool mouseClicked(unsigned int) const;
			void getMousePos(double&, double&);

			HWND getHWND() const;
		};

	}
}