#pragma once

#include <chrono>

class FPSCounter {
public:
	FPSCounter() : frame_count(0) {};

	double Count() {
		_end = std::chrono::high_resolution_clock::now();

		double fps = frame_count * 1000.0 / std::chrono::duration_cast<std::chrono::milliseconds>(_end - _start).count();

		frame_count = 0;
		_start = std::chrono::high_resolution_clock::now();

		return fps;
	}

	void Reset(){
		frame_count = 0;
		_start = std::chrono::high_resolution_clock::now();
	}

public:
	int frame_count;

private:
	std::chrono::steady_clock::time_point _start;
	std::chrono::steady_clock::time_point _end;
};