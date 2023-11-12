#pragma

#include "Types.h"

class GUIManager {
public:
	GUIManager() = default;
	void Startup(GLFWwindow* w);
	void Shutdown();
	void Draw();
};