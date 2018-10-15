#pragma once

#include <GLFW/glfw3.h>

namespace pixel {
	namespace graphics {

		void windowResize(GLFWwindow*, int, int);

		class Window {
		private:
			const char *_title;
			int _width, _height;
			GLFWwindow *_window;

			bool init();

		public:
			Window(const char *title, int width, int height);
			~Window();

			void update();
			void clear() const;
			bool closed() const;

			inline int getWidth() const { return _width;  }
			inline int getHeight() const { return _height;  }
		};
	}
}