#pragma once
#include "GraphicsManager.h"
#include "InputManager.h"
#include "spdlog/spdlog.h"
#include <memory>
#include <functional>
#include "sokol_time.h"
#include <chrono>
#include <thread>

namespace engine
{
    class Engine
    {
    public:
        int Height = 800;
        int Width = 1600;
        std::string window_name = "Huy Son's Version 1 Engine";
        bool window_fullscreen = false;

        std::shared_ptr<GraphicsManager> graphics;
        std::shared_ptr<InputManager> inputs;

        //virtual std::function<Engine> 

        Engine();
        void e_StartUp(std::shared_ptr<engine::Engine> &e);
        void e_ShutDown();
        void e_ReunGameLoop(std::function<void(std::shared_ptr<engine::Engine> &e)> UpdateCallBack); // UpdateCallback
        std::function<void(std::shared_ptr<engine::Engine>& e)> e_UpdateCallBack();
    };
};