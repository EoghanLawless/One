#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace pixel {
	namespace graphics {

#define NUM_KEYS 104
#define NUM_BUTTONS 16

		void windowResize(GLFWwindow*, int, int);

		class Window {
		private:
			GLFWwindow* _window;
			const char* _title;
			int _width, _height;

			static bool _keys[NUM_KEYS];
			static bool _buttons[NUM_BUTTONS];
			static double mouseX, mouseY;

			bool init();
			friend static void keyCallback(GLFWwindow*, int, int, int, int);

		public:
			Window(const char *title, int width, int height);
			~Window();

			void update();
			void clear() const;
			bool closed() const;

			inline int getWidth() const { return _width;  }
			inline int getHeight() const { return _height;  }

			static inline bool keyPressed(unsigned int key) { return (key < 0 || key > NUM_KEYS) ? false : _keys[key]; }
		};
	}
}