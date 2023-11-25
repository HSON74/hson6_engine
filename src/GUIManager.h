#pragma

#include "Types.h"

class GUIManager {
public:
	int g_Height = 800;
	int g_Width = 1600;
	std::string g_window_name = "Engine GUI";
	bool g_window_fullscreen = false;
	GLFWwindow* g_window;
	WGPUTextureFormat depthTextureFormat;
	WGPUTextureFormat swapChainFormat;
	WGPUInstance g_instance;
	WGPUSurface g_surface;
	WGPUAdapter g_adapter;
	WGPUDevice g_device;
	WGPUQueue g_queue;

	GUIManager() = default;
	void Startup();
	void Shutdown();
	void Draw();
};