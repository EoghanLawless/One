#pragma once

#include "graphics/window.h"

namespace pixel {

	class Pixel {
	private:
		graphics::Window* _window;

		void run() {
			while (!_window->closed()) {
				_window->clear();

				render();
				_window->update();
			}
		}

	protected:
		Pixel() { }
		virtual ~Pixel() {
			delete _window;
		}

		graphics::Window* createWindow(const char* name, int width, int height) {
			return _window = new graphics::Window(name, width, height);
		}

		void start() {
			init();
			run();
		}

		virtual void init() = 0;
		virtual void tick() = 0;
		virtual void update() = 0;
		virtual void render() = 0;

	};
}