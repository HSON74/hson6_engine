#include "GUIManager.h"
template< typename T > constexpr const T* to_ptr(const T& val) { return &val; }
template< typename T, std::size_t N > constexpr const T* to_ptr(const T(&& arr)[N]) { return arr; }
void GUIManager::Startup()
{
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOther(g_window, true);
	depthTextureFormat = WGPUTextureFormat_Depth24Plus;
	swapChainFormat = WGPUTextureFormat_BGRA8Unorm;
    g_window = glfwCreateWindow(g_Width, g_Height, g_window_name.c_str(), g_window_fullscreen ? glfwGetPrimaryMonitor() : 0, 0);
    glfwSetWindowAspectRatio(g_window, g_Width, g_Height);
    if (!g_window)
    {
        std::cerr << "Failed to create a window." << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(g_window);
    glfwSwapInterval(1);
    g_instance = wgpuCreateInstance(to_ptr(WGPUInstanceDescriptor{}));
    if (g_instance == nullptr) {
        glfwTerminate();
    }
    g_surface = glfwGetWGPUSurface(g_instance, g_window);

    g_adapter = nullptr;
    wgpuInstanceRequestAdapter(
        g_instance,
        to_ptr(WGPURequestAdapterOptions{ .compatibleSurface = g_surface }),
        [](WGPURequestAdapterStatus status, WGPUAdapter adapter, char const* message, void* adapter_ptr) {
            if (status != WGPURequestAdapterStatus_Success) {
                std::cerr << "Failed to get a WebGPU adapter: " << message << std::endl;

                glfwTerminate();
            }

            *static_cast<WGPUAdapter*>(adapter_ptr) = adapter;
        },
        &(g_adapter)
    );

    g_device = nullptr;
    wgpuAdapterRequestDevice(
        g_adapter,
        nullptr,
        [](WGPURequestDeviceStatus status, WGPUDevice device, char const* message, void* device_ptr) {
            if (status != WGPURequestDeviceStatus_Success) {
                std::cerr << "Failed to get a WebGPU device: " << message << std::endl;
                glfwTerminate();
            }

            *static_cast<WGPUDevice*>(device_ptr) = device;
        },
        &(g_device)
    );

    // An error callback to help with debugging
    wgpuDeviceSetUncapturedErrorCallback(
        g_device,
        [](WGPUErrorType type, char const* message, void*) {
            std::cerr << "WebGPU uncaptured error type " << type << " with message: " << message << std::endl;
        },
        nullptr
    );

    g_queue = wgpuDeviceGetQueue(g_device);
	//ImGui_ImplWGPU_Init(g_device, 3, swapChainFormat, depthTextureFormat);
	
	

}

void GUIManager::Shutdown()
{
    if (g_queue != NULL) { wgpuQueueRelease(g_queue); }
    if (g_device != NULL) { wgpuDeviceRelease(g_device); }
    if (g_adapter != NULL) { wgpuAdapterRelease(g_adapter); }
    if (g_surface != NULL) { wgpuSurfaceRelease(g_surface); }
    if (g_instance != NULL) { wgpuInstanceRelease(g_instance); }

	ImGui_ImplGlfw_Shutdown();
	//ImGui_ImplWGPU_Shutdown();
	ImGui::DestroyContext();
}

void GUIManager::Draw()
{
	/*ImGui_ImplWGPU_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::EndFrame(); ImGui::Render();
	ImGui_ImplWGPU_RenderDrawData();*/
}
