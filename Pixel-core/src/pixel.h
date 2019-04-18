#pragma once

#include "audio/audio.h"
#include "graphics/graphics.h"
#include "maths/maths.h"
#include "utils/utils.h"

namespace pixel {


#define UPDATES_PER_SECOND 60
#define TICKS_PER_SECOND 1

#define UPDATE_INTERVAL 1000 / UPDATES_PER_SECOND
#define TICK_INTERVAL 1 / TICKS_PER_SECOND


	class Pixel {
	private:
		graphics::Window* _window;
		Timer* _timer;

		unsigned int _fps, _ups;


		void run() {
			_timer = new Timer();
			_timer->start();

			float last_update = 0.0f;
			float last_tick = 0.0f;

			unsigned int update_counter = 0;
			unsigned int frame_counter = 0;

			while (!_window->closed()) {

				_window->clear();


				if (_timer->elapsedMilliseconds() - last_update > UPDATE_INTERVAL) {
					update();
					_window->poll();

					update_counter++;
					last_update += UPDATE_INTERVAL;
				}


				render();
				_window->update();
				frame_counter++;


				if (_timer->elapsedSeconds() - last_tick > TICK_INTERVAL) {
					_ups = update_counter;
					_fps = frame_counter;

					update_counter = 0;
					frame_counter = 0;

					last_tick += TICK_INTERVAL;

					tick();
				}
			}
		}

	protected:
		Pixel() {
			_ups = 0;
			_fps = 0;
		}
		virtual ~Pixel() {
			delete _window;
			delete _timer;
		}

		graphics::Window* createWindow(const char* name, int width, int height) {
			return _window = new graphics::Window(name, width, height);
		}

		virtual void init() = 0;
		virtual void tick() = 0;
		virtual void update() = 0;
		virtual void render() = 0;

		const unsigned int getFrames() const { return _fps; }
		const unsigned int getUpdates() const { return _ups; }

	public:
		void start() {
			init();
			run();
		}
	};

}