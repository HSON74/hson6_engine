#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

#include <iostream>
#include <string>
#include <memory>

#include "GraphicsManager.h"

void GraphicsManager::g_StartUp(int p_width, int p_height, std::string window_name, bool window_fullscreen)
{
    glfwInit();
    // We don't want GLFW to set up a graphics API.
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    // Create the window.
    GLFWwindow* window = glfwCreateWindow(p_width, p_height, window_name.c_str(), window_fullscreen ? glfwGetPrimaryMonitor() : 0, 0);
    glfwSetWindowAspectRatio(window, p_width, p_height);
    if (!window)
    {
        std::cerr << "Failed to create a window." << std::endl;
        glfwTerminate();
    }
}
void GraphicsManager::g_Shutdown()
{
    glfwTerminate();
}
void GraphicsManager::Draw() {
    std::cout << "Drawing" << std::endl;
}

