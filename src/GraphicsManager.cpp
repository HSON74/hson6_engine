
#define STB_IMAGE_IMPLEMENTATION 100
#include "stb_image.h"
#include "GraphicsManager.h"


using namespace glm;
using namespace m_Types;

// A vertex buffer containing a textured square.
const struct {
    // position
    float x, y;
    // texcoords
    float u, v;
} vertices[] = {
    // position       // texcoords
  { -1.0f,  -1.0f,    0.0f,  1.0f },
  {  1.0f,  -1.0f,    1.0f,  1.0f },
  { -1.0f,   1.0f,    0.0f,  0.0f },
  {  1.0f,   1.0f,    1.0f,  0.0f },
};

namespace {
    // Declaration here
    struct InstanceData {
        glm::vec3 translation;
        glm::vec3 scale;
        glm::vec3 rotation;
    };
    struct Uniforms {
        mat4 projection;
    };
    
}

template< typename T > constexpr const T* to_ptr(const T& val) { return &val; }
template< typename T, std::size_t N > constexpr const T* to_ptr(const T(&& arr)[N]) { return arr; }
std::shared_ptr<ResourceManager> ecsrr = std::make_shared<ResourceManager>();

void GraphicsManager::g_StartUp()
{
    resources->StartUp();
    glfwInit();
    // We don't want GLFW to set up a graphics API.
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    // Create the window.
    window = glfwCreateWindow(Width, Height, window_name.c_str(), window_fullscreen ? glfwGetPrimaryMonitor() : 0, 0);
    glfwSetWindowAspectRatio(window, Width, Height);
    if (!window)
    {
        std::cerr << "Failed to create a window." << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    instance = wgpuCreateInstance(to_ptr(WGPUInstanceDescriptor{}));
    if (instance == nullptr) {
        glfwTerminate();
    }
    surface = glfwGetWGPUSurface(instance, window);

    adapter = nullptr;
    wgpuInstanceRequestAdapter(
        instance,
        to_ptr(WGPURequestAdapterOptions{.compatibleSurface = surface }),
        [](WGPURequestAdapterStatus status, WGPUAdapter adapter, char const* message, void* adapter_ptr) {
            if (status != WGPURequestAdapterStatus_Success) {
                std::cerr << "Failed to get a WebGPU adapter: " << message << std::endl;

                glfwTerminate();
            }

            *static_cast<WGPUAdapter*>(adapter_ptr) = adapter;
        },
        &(adapter)
    );

    device = nullptr;
    wgpuAdapterRequestDevice(
        adapter,
        nullptr,
        [](WGPURequestDeviceStatus status, WGPUDevice device, char const* message, void* device_ptr) {
            if (status != WGPURequestDeviceStatus_Success) {
                std::cerr << "Failed to get a WebGPU device: " << message << std::endl;
                glfwTerminate();
            }

            *static_cast<WGPUDevice*>(device_ptr) = device;
        },
        &(device)
    );

    // An error callback to help with debugging
    wgpuDeviceSetUncapturedErrorCallback(
        device,
        [](WGPUErrorType type, char const* message, void*) {
            std::cerr << "WebGPU uncaptured error type " << type << " with message: " << message << std::endl;
        },
        nullptr
    );
    
    queue = wgpuDeviceGetQueue(device);
    
    //Write the vertex_buffer
    vertex_buffer = wgpuDeviceCreateBuffer(device, to_ptr(WGPUBufferDescriptor{
    .usage = WGPUBufferUsage_CopyDst | WGPUBufferUsage_Vertex,
    .size = sizeof(vertices)
        }));

    wgpuQueueWriteBuffer(queue, vertex_buffer, 0, vertices, sizeof(vertices));
    
    swap_chain_format = wgpuSurfaceGetPreferredFormat(surface, adapter);
    glfwGetFramebufferSize(window, &Width, &Height);
    swapchain = wgpuDeviceCreateSwapChain(device, surface, to_ptr(WGPUSwapChainDescriptor{
        .usage = WGPUTextureUsage_RenderAttachment,
        .format = swap_chain_format,
        .width = (uint32_t)Width,
        .height = (uint32_t)Height
        }));

    texSampler = wgpuDeviceCreateSampler(device, to_ptr(WGPUSamplerDescriptor{
    .addressModeU = WGPUAddressMode_ClampToEdge,
    .addressModeV = WGPUAddressMode_ClampToEdge,
    .magFilter = WGPUFilterMode_Linear,
    .minFilter = WGPUFilterMode_Linear,
    .maxAnisotropy = 1
        }));
   const char* source = R"(
    struct Uniforms {
    projection: mat4x4f,
};

@group(0) @binding(0) var<uniform> uniforms: Uniforms;
@group(0) @binding(1) var texSampler: sampler;
@group(0) @binding(2) var texData: texture_2d<f32>;

struct VertexInput {
    @location(0) position: vec2f,
    @location(1) texcoords: vec2f,
    @location(2) translation: vec3f,
    @location(3) scale: vec3f,
    @location(4) rotation: vec3f,
};

struct VertexOutput {
    @builtin(position) position: vec4f,
    @location(0) texcoords: vec2f,
};

@vertex
fn vertex_shader_main( in: VertexInput ) -> VertexOutput {
    var out: VertexOutput;
    out.position = uniforms.projection * vec4f( vec3f( in.scale * vec3f(in.position,1.0)) + in.translation, 1.0 );
   
    out.texcoords = in.texcoords;
    return out;
}

@fragment
fn fragment_shader_main( in: VertexOutput ) -> @location(0) vec4f {
    let color = textureSample( texData, texSampler, in.texcoords ).rgba;
    return color;
}
    )";
   WGPUShaderModuleWGSLDescriptor code_desc = {};
    code_desc.chain.sType = WGPUSType_ShaderModuleWGSLDescriptor;
    code_desc.code = source; // The shader source as a `char*`
    WGPUShaderModuleDescriptor shader_desc = {};
    shader_desc.nextInChain = &code_desc.chain;
    shader_module = wgpuDeviceCreateShaderModule(device, &shader_desc);

    pipeline = wgpuDeviceCreateRenderPipeline(device, to_ptr(WGPURenderPipelineDescriptor{

        // Describe the vertex shader inputs
        .vertex = {
            .module = shader_module,
            .entryPoint = "vertex_shader_main",
            // Vertex attributes.
            .bufferCount = 2,
            .buffers = to_ptr<WGPUVertexBufferLayout>({
            // We have one buffer with our per-vertex position and UV data. This data never changes.
            // Note how the type, byte offset, and stride (bytes between elements) exactly matches our `vertex_buffer`.
            {
                .arrayStride = 4 * sizeof(float),
                .attributeCount = 2,
                .attributes = to_ptr<WGPUVertexAttribute>({
                    // Position x,y are first.
                    {
                        .format = WGPUVertexFormat_Float32x2,
                        .offset = 0,
                        .shaderLocation = 0
                    },
                    // Texture coordinates u,v are second.
                    {
                        .format = WGPUVertexFormat_Float32x2,
                        .offset = 2 * sizeof(float),
                        .shaderLocation = 1
                    }
                    })
            },
                // We will use a second buffer with our per-sprite translation and scale. This data will be set in our draw function.
                {
                    .arrayStride = sizeof(InstanceData),
                    // This data is per-instance. All four vertices will get the same value. Each instance of drawing the vertices will get a different value.
                    // The type, byte offset, and stride (bytes between elements) exactly match the array of `InstanceData` structs we will upload in our draw function.
                    .stepMode = WGPUVertexStepMode_Instance,
                    .attributeCount = 3,
                    .attributes = to_ptr<WGPUVertexAttribute>({
                    // Translation as a 3D vector.
                    {
                        .format = WGPUVertexFormat_Float32x3,
                        .offset = offsetof(InstanceData, translation),
                        .shaderLocation = 2
                    },
                        // Scale as a 2D vector for non-uniform scaling.
                        {
                            .format = WGPUVertexFormat_Float32x2,
                            .offset = offsetof(InstanceData, scale),
                            .shaderLocation = 3
                        },
                        {
                            .format = WGPUVertexFormat_Float32x2,
                            .offset = offsetof(InstanceData, rotation),
                            .shaderLocation = 4
                        }
                        })
                }
                })
            },

        // Interpret our 4 vertices as a triangle strip
        .primitive = WGPUPrimitiveState{
            .topology = WGPUPrimitiveTopology_TriangleStrip,
            },

            // No multi-sampling (1 sample per pixel, all bits on).
            .multisample = WGPUMultisampleState{
                .count = 1,
                .mask = ~0u
                },

        // Describe the fragment shader and its output
        .fragment = to_ptr(WGPUFragmentState{
            .module = shader_module,
            .entryPoint = "fragment_shader_main",

            // Our fragment shader outputs a single color value per pixel.
            .targetCount = 1,
            .targets = to_ptr<WGPUColorTargetState>({
                {
                    .format = swap_chain_format,
                    // The images we want to draw may have transparency, so let's turn on alpha blending with over compositing (ɑ⋅foreground + (1-ɑ)⋅background).
                    // This will blend with whatever has already been drawn.
                    .blend = to_ptr(WGPUBlendState{
                    // Over blending for color
                    .color = {
                        .operation = WGPUBlendOperation_Add,
                        .srcFactor = WGPUBlendFactor_SrcAlpha,
                        .dstFactor = WGPUBlendFactor_OneMinusSrcAlpha
                        },
                        // Leave destination alpha alone
                        .alpha = {
                            .operation = WGPUBlendOperation_Add,
                            .srcFactor = WGPUBlendFactor_Zero,
                            .dstFactor = WGPUBlendFactor_One
                            }
                        }),
                    .writeMask = WGPUColorWriteMask_All
                }})
            })
        }));
    uniform_buffer = wgpuDeviceCreateBuffer(device, to_ptr(WGPUBufferDescriptor{
        .usage = WGPUBufferUsage_CopyDst | WGPUBufferUsage_Uniform,
        .size = sizeof(Uniforms)
        }));
    for (int i = 0; i < 10; i++) {
        std::string n_tmp = std::to_string(i);
        std::string path_tmp  = ecsrr->pathToString(ecsrr->getCurrentPath() / ecsrr->stringToPath("assets\\UIText") / ecsrr->stringToPath(n_tmp + ".png"));
        LoadUI(n_tmp, path_tmp);
    }
};
GraphicsManager::GraphicsManager()
{
    Height = 800;
    Width = 1600;
    window_name = "Huy Son's Version 1 Engine";
    window_fullscreen = false;
    resources = std::make_shared<ResourceManager>();
};
void GraphicsManager::changeBackground(float r1, float g1, float b1) {
    this->red = r1;
    this->green = g1;
    this->blue = b1;
}

void GraphicsManager::resize() {

   
    glfwSetFramebufferSizeCallback(this->window, [](GLFWwindow* window, int width, int height)
        {
           
            glfwGetFramebufferSize(window, &width, &height);
           
        });
   glfwGetFramebufferSize(window, &Width, &Height);
   if (Height < 0 || Width < 0) {
       if (swapchain) { wgpuSwapChainRelease(swapchain); }
       swapchain = wgpuDeviceCreateSwapChain(device, surface, to_ptr(WGPUSwapChainDescriptor{
               .usage = WGPUTextureUsage_RenderAttachment,
               .format = swap_chain_format,
               .width = (uint32_t)Width,
               .height = (uint32_t)Height
           }));
   }
};
bool GraphicsManager::LoadUI(const std::string& name, const std::string& path)
{
    int width, height, channels;
    //std::string t_path = this->resources->rSPath + path;
    

    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 4);
    if (data == NULL) {
        return false;
    }
    WGPUTexture tex = wgpuDeviceCreateTexture(device, to_ptr(WGPUTextureDescriptor{
        .usage = WGPUTextureUsage_TextureBinding | WGPUTextureUsage_CopyDst,
        .dimension = WGPUTextureDimension_2D,
        .size = { (uint32_t)width, (uint32_t)height, 1 },
        .format = WGPUTextureFormat_RGBA8Unorm,
        .mipLevelCount = 1,
        .sampleCount = 1
        }));
    wgpuQueueWriteTexture(
        queue,
        to_ptr<WGPUImageCopyTexture>({ .texture = tex }),
        data,
        width * height * 4,
        to_ptr<WGPUTextureDataLayout>({ .bytesPerRow = (uint32_t)(width * 4), .rowsPerImage = (uint32_t)height }),
        to_ptr(WGPUExtent3D{ (uint32_t)width, (uint32_t)height, 1 })
    );
    HUYSONSOMETHING tmpSomething;
    tmpSomething.name = name;
    tmpSomething.path = path;
    tmpSomething.height = height;
    tmpSomething.width = width;
    tmpSomething.channels = channels;
    tmpSomething.image_tex = tex;
    stbi_image_free(data);
    g_UI[name] = tmpSomething;
    return true;
}
bool GraphicsManager::LoadFrame(const std::string& name, const std::string& path)
{
    int width, height, channels;
    //std::string t_path = this->resources->rSPath + path;


    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 4);
    if (data == NULL) {
        return false;
    }
    WGPUTexture tex = wgpuDeviceCreateTexture(device, to_ptr(WGPUTextureDescriptor{
        .usage = WGPUTextureUsage_TextureBinding | WGPUTextureUsage_CopyDst,
        .dimension = WGPUTextureDimension_2D,
        .size = { (uint32_t)width, (uint32_t)height, 1 },
        .format = WGPUTextureFormat_RGBA8Unorm,
        .mipLevelCount = 1,
        .sampleCount = 1
        }));
    wgpuQueueWriteTexture(
        queue,
        to_ptr<WGPUImageCopyTexture>({ .texture = tex }),
        data,
        width * height * 4,
        to_ptr<WGPUTextureDataLayout>({ .bytesPerRow = (uint32_t)(width * 4), .rowsPerImage = (uint32_t)height }),
        to_ptr(WGPUExtent3D{ (uint32_t)width, (uint32_t)height, 1 })
    );
    g_tex[name].path = path;
    g_tex[name].height = width;
    g_tex[name].width = channels;
    g_tex[name].image_tex = tex;
    g_tex[name].channels = channels;
    stbi_image_free(data);
    
    return true;
}
;
bool GraphicsManager::LoadImage(const std::string& name, const std::string& path)
{
    int width, height, channels;
    //std::string t_path = this->resources->rSPath + path;
   
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 4);
    if (data == NULL) {
        std::cout << name << "don't exist" << std::endl;
        return false;
    }
    WGPUTexture tex = wgpuDeviceCreateTexture(device, to_ptr(WGPUTextureDescriptor{
        .usage = WGPUTextureUsage_TextureBinding | WGPUTextureUsage_CopyDst,
        .dimension = WGPUTextureDimension_2D,
        .size = { (uint32_t)width, (uint32_t)height, 1 },
        .format = WGPUTextureFormat_RGBA8Unorm,
        .mipLevelCount = 1,
        .sampleCount = 1
        }));
    wgpuQueueWriteTexture(
        queue,
        to_ptr<WGPUImageCopyTexture>({ .texture = tex }),
        data,
        width * height * 4,
        to_ptr<WGPUTextureDataLayout>({ .bytesPerRow = (uint32_t)(width * 4), .rowsPerImage = (uint32_t)height }),
        to_ptr(WGPUExtent3D{ (uint32_t)width, (uint32_t)height, 1 })
    );
    HUYSONSOMETHING tmpSomething;
    tmpSomething.name = name;
    tmpSomething.path = path;
    tmpSomething.height = height;
    tmpSomething.width = width;
    tmpSomething.channels = channels;
    tmpSomething.image_tex = tex;
    stbi_image_free(data);
    g_tex[name] = tmpSomething;
    return true;
};


void GraphicsManager::g_Shutdown()
{
    if (!g_tex.empty()) {
        for (auto m_tex = g_tex.begin(); m_tex != g_tex.end(); m_tex++) {
            auto cur = m_tex->first;
            g_tex[cur].destoryit();
        }
        g_tex.clear();
        
    }
    if (!g_UI.empty()) {
        for (auto m_tex = g_UI.begin(); m_tex != g_UI.end(); m_tex++) {
            auto cur = m_tex->first;
            g_UI[cur].destoryit();
        }
        g_UI.clear();

    }
    if (sprites.size() != 0) { sprites.clear(); }
    if (UI_sprites.size() != 0) {
        UI_sprites.clear();
    }
    if (pipeline != NULL) { wgpuRenderPipelineRelease(pipeline); }
    if (shader_module != NULL) { wgpuShaderModuleRelease(shader_module); }
    if (texSampler != NULL) { wgpuSamplerRelease(texSampler); }
    if (uniform_buffer != NULL) { wgpuBufferRelease(uniform_buffer); }
    if (swapchain != nullptr) { wgpuSwapChainRelease(swapchain); }
    if (queue != NULL) { wgpuQueueRelease(queue); }
    if (device != NULL) { wgpuDeviceRelease(device); }
    if (adapter != NULL) { wgpuAdapterRelease(adapter); }
    if (surface != NULL) { wgpuSurfaceRelease(surface); }
    if (instance != NULL) { wgpuInstanceRelease(instance); }
    resources->Shutdown();
    glfwTerminate();
};
void GraphicsManager::ShouldQuit() {
    std::cerr << "window is close" << std::endl;
    window_isRunning = false;
   
    glfwSetWindowShouldClose(window, 1);
}
glm::mat4 GraphicsManager::multmatrix(glm::mat4 a, glm::mat4 b)
{
    glm::mat4 result;
    for (int h = 0; h < 4; h++) {
        for (int u = 0; u < 4; u++) {
            result[h][u] = 0.0f;
        }
    }
    for (int h = 0; h < 4; h++) {
        for (int u = 0; u < 4; u++) {
            for (int y = 0; y < 4; y++) {
                result[h][u] += a[h][y] * b[y][u];
            }
        }
    }
    return result;
}
;

void GraphicsManager::Draw(std::vector<Sprite>& sprites, std::vector<UI>& UI_sprites) {
    
    //std::cout << "Drawing" << std::endl;
    WGPUCommandEncoder encoder = wgpuDeviceCreateCommandEncoder(device, nullptr);

    WGPUTextureView current_texture_view = wgpuSwapChainGetCurrentTextureView(swapchain);

    WGPURenderPassEncoder render_pass = wgpuCommandEncoderBeginRenderPass(encoder, to_ptr<WGPURenderPassDescriptor>({
    .colorAttachmentCount = 1,
    .colorAttachments = to_ptr<WGPURenderPassColorAttachment>({{
        .view = current_texture_view,
        .loadOp = WGPULoadOp_Clear,
        .storeOp = WGPUStoreOp_Store,
        // Choose the background color.
        .clearValue = WGPUColor{ red, green, blue, 1.0 }
        }})
        }));

    wgpuRenderPassEncoderSetPipeline(render_pass, pipeline);
    

     Uniforms uniforms;
    // Start with an identity matrix.
    //uniforms.projection = mat4{1};
    uniforms.projection = mat4{1};
    // Scale x and y by 1/100.
    uniforms.projection[0][0] = uniforms.projection[1][1] = 1. / 100.;
    /*// Scale the long edge by an additional 1/(long/short) = short/long.
    if (Width < Height) {
        uniforms.projection[1][1] *= Width;
        uniforms.projection[1][1] /= Height;
    }
    else {
        uniforms.projection[0][0] *= Height;
        uniforms.projection[0][0] /= Width;
    } */
    if (Width < Height) {
        uniforms.projection = mat4({ 2.0f / (Height - (-Height)), 0, 0, -((Height + (-Height))) / ((Height - (-Height))) },
            { 0, 2.0f / (Width - (-Width)), 0, -(Width + (-Width)) / (Width - (-Width)) },
            { 0, 0, -2.0f / ((Height - (-Height))), -((Height + (-Height))) / ((Height - (-Height))) },
            { 0, 0, 0, 1 });
    }
    else {
        uniforms.projection = mat4({ 2.0f / (Width - (-Width)), 0, 0, -((Width + (-Width))) / ((Width - (-Width))) },
            { 0, 2.0f / (Height - (-Height)), 0, -(Height + (-Height)) / (Height - (-Height)) },
            { 0, 0, -2.0f / ((Width - (-Width))), -((Width + (-Width))) / ((Width - (-Width))) },
            { 0, 0, 0, 1 });
    }
    wgpuQueueWriteBuffer(queue, uniform_buffer, 0, &uniforms, sizeof(Uniforms));
    WGPUBindGroup bind_group = nullptr;
    if (sprites.size() > 0) {

        instance_buffer = wgpuDeviceCreateBuffer(device, to_ptr<WGPUBufferDescriptor>({
           .usage = WGPUBufferUsage_CopyDst | WGPUBufferUsage_Vertex,
           .size = sizeof(InstanceData) * sprites.size()
            }));
        wgpuRenderPassEncoderSetVertexBuffer(render_pass, 0, vertex_buffer, 0, 4 * 4 * sizeof(float));
        wgpuRenderPassEncoderSetVertexBuffer(render_pass, 1, instance_buffer, 0, sizeof(InstanceData) * sprites.size());

        std::sort(sprites.begin(), sprites.end(), [](const Sprite& lhs, const Sprite& rhs) { return lhs.z_value > rhs.z_value; });
        for (int i = 0; i < sprites.size(); i++) {
            
            if (!g_tex.contains(sprites.at(i).imageName) && !g_UI.contains(UI_sprites.at(sprites.at(i).UI_location).tex_text)) { continue; }
            InstanceData d;
            Sprite s = sprites.at(i);
            
            if (s.tag == "UI" && !s.active) { 
                
                continue; 
            }
           d.translation = vec3((float)((float)s.position.x), (float)((float)s.position.y), (float)s.position.z);
           
           d.scale = vec3(s.scale.x * 100, s.scale.y * 100, s.scale.z*100);
           //if (s.EntityN == 0) {
             //  std::cout << "x: " << s.scale.x << "y: " << s.scale.y << "z: " << s.scale.z << std::endl;
           //}
           float angle_x = (float)(s.rotation.x * 3.14) / 180.0f;
           float angle_y = (float)(s.rotation.y * 3.14) / 180.0f;
           float angle_z = (float)(s.rotation.z * 3.14) / 180.0f;
           glm::mat4 R = { {cos(angle_z),sin(angle_z),0,0},
                            { -sin(angle_z),cos(angle_z),0,0},
                            {0,0,1,0},
                            {0,0,0,1}};
           glm::vec4 ttc = R * glm::vec4(s.scale.x * 100, s.scale.y * 100, s.scale.z * 100, 1);
           d.rotation = vec3(ttc.x, ttc.y, ttc.z);
           wgpuQueueWriteBuffer(queue, instance_buffer, i * sizeof(InstanceData), &d, sizeof(InstanceData));
           WGPUTexture tex_tmp;
         
           if (sprites.at(i).tag == "UI" && UI_sprites.at(sprites.at(i).UI_location).tex_text != "") {
              
               tex_tmp = g_UI[UI_sprites.at(sprites.at(i).UI_location).tex_text].image_tex;
           }
           else {
               tex_tmp = g_tex[sprites.at(i).imageName].image_tex;
           }
            auto layout = wgpuRenderPipelineGetBindGroupLayout(pipeline, 0);
            bind_group = wgpuDeviceCreateBindGroup(device, to_ptr(WGPUBindGroupDescriptor{
                .layout = layout,
                .entryCount = 3,
                // The entries `.binding` matches what we wrote in the shader.
                .entries = to_ptr<WGPUBindGroupEntry>({
                    {
                        .binding = 0,
                        .buffer = uniform_buffer,
                        .size = sizeof(Uniforms)
                    },
                    {
                        .binding = 1,
                        .sampler = texSampler,
                    },
                    {
                        .binding = 2,
                        .textureView = wgpuTextureCreateView(tex_tmp, nullptr)
                    }
                })
            }));

            wgpuBindGroupLayoutRelease(layout);
            wgpuRenderPassEncoderSetBindGroup(render_pass, 0, bind_group, 0, nullptr);
            wgpuRenderPassEncoderDraw(render_pass, 4, 1, 0, i);

        }
        
    }
    wgpuRenderPassEncoderEnd(render_pass);

    WGPUCommandBuffer command = wgpuCommandEncoderFinish(encoder, nullptr);
    wgpuQueueSubmit(queue, 1, &command);
    wgpuSwapChainPresent(swapchain);

    if (bind_group) {
        wgpuBindGroupRelease(bind_group); 
    }

    if (instance_buffer) {
        wgpuBufferRelease(instance_buffer);
    }

    if (render_pass) {
        wgpuRenderPassEncoderRelease(render_pass);
    }

    if (current_texture_view) {
        wgpuTextureViewRelease(current_texture_view);
    }

    if (encoder) {
        wgpuCommandEncoderRelease(encoder);
    }
    
};

