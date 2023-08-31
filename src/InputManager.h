#pragma

#include "sokol_time.h"
#include <chrono>
#include <thread>

class InputManager
{
	public:
		int input_num = 0;
		void Update();
		InputManager() = default;
};