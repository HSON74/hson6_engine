#include "GUIManager.h"
template< typename T > constexpr const T* to_ptr(const T& val) { return &val; }
template< typename T, std::size_t N > constexpr const T* to_ptr(const T(&& arr)[N]) { return arr; }
void GUIManager::Startup()
{
	ImGui::CreateContext();
	
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
    ImGui_ImplGlfw_InitForOther(g_window, true);
	ImGui_ImplWGPU_Init(g_device, 3, swapChainFormat, depthTextureFormat);
	
	

}
void GUIManager::ShouldQuit() {

    //Shutdown();
    glfwSetWindowShouldClose(g_window, 1);
    
    
}
void GUIManager::Shutdown()
{
    if (g_queue != NULL) { wgpuQueueRelease(g_queue); }
    if (g_device != NULL) { wgpuDeviceRelease(g_device); }
    if (g_adapter != NULL) { wgpuAdapterRelease(g_adapter); }
    if (g_surface != NULL) { wgpuSurfaceRelease(g_surface); }
    if (g_instance != NULL) { wgpuInstanceRelease(g_instance); }

	ImGui_ImplGlfw_Shutdown();
	ImGui_ImplWGPU_Shutdown();
	ImGui::DestroyContext();
}

void GUIManager::Draw()
{
    glfwGetFramebufferSize((GLFWwindow*)g_window, &g_Width, &g_Height);

    // React to changes in screen size
    if (g_Width != wgpu_swap_chain_width && g_Height != wgpu_swap_chain_height)
    {
        ImGui_ImplWGPU_InvalidateDeviceObjects();
        if (wgpu_swap_chain)
            wgpuSwapChainRelease(wgpu_swap_chain);
        wgpu_swap_chain_width = g_Width;
        wgpu_swap_chain_height = g_Height;
        WGPUSwapChainDescriptor swap_chain_desc = {};
        swap_chain_desc.usage = WGPUTextureUsage_RenderAttachment;
        swap_chain_desc.format = wgpu_preferred_fmt;
        swap_chain_desc.width = g_Width;
        swap_chain_desc.height = g_Height;
        swap_chain_desc.presentMode = WGPUPresentMode_Fifo;
        wgpu_swap_chain = wgpuDeviceCreateSwapChain(g_device, g_surface, &swap_chain_desc);
        ImGui_ImplWGPU_CreateDeviceObjects();
    }
	ImGui_ImplWGPU_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::EndFrame(); 
    static bool show_demo_window = true;
    static bool show_another_window = false;
    static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    ImGuiIO& io = ImGui::GetIO();

    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Hello, world!");                                // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");                     // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window", &show_demo_window);            // Edit bools storing our window open/close state
        ImGui::Checkbox("Another Window", &show_another_window);

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);                  // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color", (float*)&clear_color);       // Edit 3 floats representing a color

        if (ImGui::Button("Button"))                                  // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();
    }

    // 3. Show another simple window.
    if (show_another_window)
    {
        ImGui::Begin("Another Window", &show_another_window);         // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me"))
            show_another_window = false;
        ImGui::End();
    }

    // Rendering
    ImGui::Render();

    WGPURenderPassColorAttachment color_attachments = {};
    color_attachments.loadOp = WGPULoadOp_Clear;
    color_attachments.storeOp = WGPUStoreOp_Store;
    color_attachments.clearValue = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
    color_attachments.view = wgpuSwapChainGetCurrentTextureView(wgpu_swap_chain);
    WGPURenderPassDescriptor render_pass_desc = {};
    render_pass_desc.colorAttachmentCount = 1;
    render_pass_desc.colorAttachments = &color_attachments;
    render_pass_desc.depthStencilAttachment = nullptr;

    WGPUCommandEncoderDescriptor enc_desc = {};
    WGPUCommandEncoder encoder = wgpuDeviceCreateCommandEncoder(g_device, &enc_desc);

    WGPURenderPassEncoder pass = wgpuCommandEncoderBeginRenderPass(encoder, &render_pass_desc);
    ImGui_ImplWGPU_RenderDrawData(ImGui::GetDrawData(), pass);
    wgpuRenderPassEncoderEnd(pass);

    WGPUCommandBufferDescriptor cmd_buffer_desc = {};
    WGPUCommandBuffer cmd_buffer = wgpuCommandEncoderFinish(encoder, &cmd_buffer_desc);
    WGPUQueue queue = wgpuDeviceGetQueue(g_device);
    wgpuQueueSubmit(queue, 1, &cmd_buffer);
}
   // ImGui::Render();
	//ImGui_ImplWGPU_RenderDrawData();
