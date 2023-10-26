#pragma
#include "Types.h"

class InputManager
{
	public:
		int inputKey;
		int key_input;
		void Update();
		void StartUp();
		bool KeyIsPressed(GLFWwindow* window, int key, int action);
		InputManager() = default;
};
