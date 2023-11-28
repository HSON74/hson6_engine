#pragma

#include "Types.h"

class GUIManager {
public:
	int g_Height = 1600;
	int g_Width = 400;
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
	WGPUTextureFormat wgpu_preferred_fmt = WGPUTextureFormat_RGBA8Unorm;
	WGPUSwapChain     wgpu_swap_chain = nullptr;
	int               wgpu_swap_chain_width = 400;
	int               wgpu_swap_chain_height = 1600;

	GUIManager() = default;
	void Startup();
	void Shutdown();
	void Draw();
	void ShouldQuit();
};