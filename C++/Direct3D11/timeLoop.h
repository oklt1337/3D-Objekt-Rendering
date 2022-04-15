#pragma once
#include <Windows.h>
#include <chrono>

using namespace std;

#define NOW chrono::high_resolution_clock::now()
typedef chrono::high_resolution_clock::time_point time_point;
typedef chrono::duration<float> duration;

class time_loop
{
public:
	INT init();
	void update();
	static void de_init();

	FLOAT get_delta_time() const { return delta_time_; }

private:
	time_point last_timestamp_ = {};

	FLOAT delta_time_ = 0.0f;
	FLOAT total_time_ = 0.0f;
	UINT fps_ = 0;
	UINT last_fps_ = 0;
	FLOAT fps_time_ = 0.0f;
};

