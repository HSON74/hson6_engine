#pragma once
#include "GraphicsManager.h"
#include "InputManager.h"
#include "SoundManager.h"
#include "ScriptsManager.h"
#include "Types.h"
#include "ECS.h"
#include "GUIManager.h"


namespace engine
{
    class Engine
    {
    public:
        typedef std::function<void()> e_UpdateCallback;

        std::shared_ptr<GraphicsManager> graphics;
        std::shared_ptr<InputManager> inputs;
        std::shared_ptr<SoundManager> sound;
        std::shared_ptr<ScriptsManager> e_script;
        std::shared_ptr<ECS> ecs;
        std::shared_ptr<GUIManager> gui;
        bool u_result = false;
        

        //virtual std::function<Engine> 

        Engine();
        void e_StartUp(std::shared_ptr<engine::Engine> &e);
        void e_ShutDown();
        void e_ReunGameLoop(const e_UpdateCallback& callback); // UpdateCallback
        void e_my_function();
        bool GetKeyDown(int key);
        void e_Update();
        void HelloUser();
        void UserInput(std::shared_ptr<engine::Engine>& e);
        void EngineForEach();
        void addScript(EntityID e, const std::string name);
        void addECSEnvironment(EntityID e);
    };
};