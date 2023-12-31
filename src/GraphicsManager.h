#pragma

#include "Types.h"
#include "ResourceManager.h"



struct HUYSONSOMETHING {
    std::string name;
    std::string path;
    int width;
    int height; 
    int channels;
    WGPUTexture image_tex;

    void destoryit() {
        if (image_tex != nullptr) {
            wgpuTextureDestroy(image_tex);
            wgpuTextureRelease(image_tex);
            image_tex = nullptr;
        }
    };
};

class GraphicsManager
{
public:
    int Height = 800;
    int Width = 1600;
    std::string window_name = "Huy Son's Version 1 Engine";
    bool window_fullscreen = false;
    GLFWwindow* window;
    WGPUInstance instance;
    WGPUSurface surface;
    WGPUAdapter adapter;
    WGPUDevice device;
    WGPUQueue queue;
    WGPUBuffer vertex_buffer;
    WGPUBuffer instance_buffer;
    WGPUSwapChain swapchain;
    WGPUBuffer uniform_buffer;
    WGPUSampler texSampler;
    WGPUShaderModule shader_module;
    WGPURenderPipeline pipeline;
    WGPUTextureFormat swap_chain_format;
    std::vector<Sprite> sprites;
    std::vector<UI> UI_sprites;
    std::unordered_map<std::string, HUYSONSOMETHING> g_tex;
    std::unordered_map<std::string, HUYSONSOMETHING> g_UI;
    std::shared_ptr<ResourceManager> resources;
    float red = 0, green = 0, blue = 0;
    

    bool window_isRunning;
    void g_StartUp();
    void g_Shutdown();
    void Draw(const std::shared_ptr<Camera>& cam,std::vector<Sprite>& sprites, std::vector<UI>& UI_sprites);
    friend class InputManager;
    void changeBackground(float r1, float g1, float b1);
    GraphicsManager();
    void resize();
    bool LoadImage(const std::string& name, const std::string& path);
    bool LoadUI(const std::string& name, const std::string& path);
    bool LoadFrame(const std::string& name, const std::string& path);
    void ShouldQuit();
    glm::mat4 multmatrix(glm::mat4 a, glm::mat4);
};