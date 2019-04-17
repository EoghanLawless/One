#pragma once

#include <iostream>
#include <chrono>
#include <ctime>
#include <cmath>

class Timer {
private:
	std::chrono::time_point<std::chrono::system_clock> _start;
	std::chrono::time_point<std::chrono::system_clock> _end;
	bool _running = false;

public:
	Timer& start() {
		if (!_running)
			_start = std::chrono::system_clock::now();

		_running = true;

		return *this;
	}
	void restart() {
		_start = std::chrono::system_clock::now();
		_running = true;
	}
	void stop() {
		if(_running)
			_end = std::chrono::system_clock::now();

		_running = false;
	}

	float elapsedMilliseconds() {
		std::chrono::time_point<std::chrono::system_clock> end;

		if (_running)
			end = std::chrono::system_clock::now();
		else
			end = _end;

		return std::chrono::duration_cast<std::chrono::milliseconds>(end - _start).count();
	}
	float elapsedSeconds() {
		return elapsedMilliseconds() / 1000.0;
	}
};
