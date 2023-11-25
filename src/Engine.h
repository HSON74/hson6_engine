#pragma once
#include "GraphicsManager.h"
#include "InputManager.h"
#include "SoundManager.h"
#include "ScriptsManager.h"
#include "Types.h"
#include "ECS.h"
#include "GUIManager.h"
#include "AnimatorManager.h"


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
        std::shared_ptr<AnimatorManager> animation;
        std::shared_ptr<Camera> cam;
        bool u_result = false;

        bool isPlay = false;
        bool Reboot = true;
        bool isPlayMode = false;
        bool is_Test = false;
        int jump_count = 0;
        bool isJump = false;
        bool isFlipLeft = false;

        //virtual std::function<Engine> 

        Engine();
        void e_StartUp(std::shared_ptr<engine::Engine> &e);
        void e_ShutDown();
        void e_perp(std::shared_ptr<Camera>& cam);
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