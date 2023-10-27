
#include "Engine.h"
#define CAT(x, y) CAT_(x, y)
#define CAT_(x, y) x ## y
/*#define STR(x) #x
#define COMPONENT_TEMPLATE(type) {\
    lua.set_function(CAT("Get",STR(type)), [&](EntityID e){return ECS::Get<type>(e)});\
}*/


using namespace m_Types;

void engine::Engine::e_StartUp(std::shared_ptr<engine::Engine> &e)
{
	graphics->g_StartUp();
	inputs->StartUp();
	sound->StartUp();
	e_script->StartUp();
	//ECS
	e_script->lua.set_function("CreateObject", [&](const std::string name, const std::string path) {ecs->Create(name, path); });
	e_script->lua.set_function("Destroy", [&](const std::string name) {
		auto& container = ecs->GetAppropriateSparseSet<Sprite>();
		for (const auto& [entity, value] : container) {
			Sprite s = ecs->Get<Sprite>(entity);
			if (name._Equal(s.imageName)) {
				if (s.active) {
					for (int i = 0; i < this->graphics->sprites.size(); i++) {
						if (s.imageName._Equal(this->graphics->sprites.at(i).imageName)) {
							std::string a = this->graphics->sprites.at(i).imageName;
							this->graphics->g_tex[a].destoryit();
							this->graphics->g_tex.erase(a);
							this->graphics->sprites.erase(this->graphics->sprites.begin() + i);
						}
					}
				}
				ecs->Destroy(entity);
			}
			
		}
		});
	e_script->lua.set_function("GetSprite", [&](EntityID e) -> Sprite& { return ecs->Get<Sprite>(e); });
	e_script->lua.set_function("SetHealth", [&](EntityID e, double p) {ecs->Get<Health>(e).percent = p; });
	e_script->lua.set_function("SetPosition", [&](EntityID e, float x, float y, float z) {ecs->Get<Position>(e).x += x; ecs->Get<Position>(e).y += y; ecs->Get<Position>(e).z += z; });
	
	e_script->lua.set_function("SetScale", [&](EntityID e, float x, float y) {ecs->Get<Sprite>(e).scale.x = x; ecs->Get<Sprite>(e).scale.y += y;});
	e_script->lua.set_function("Collide", [&](EntityID e, std::string tag) {return ecs->Collider(e, tag); });
	
	e_script->lua.set_function("AddScript", [&](EntityID e, const std::string name){addScript(e, name); });
	e_script->lua.set_function("GetScript", [&](EntityID e) -> Script& {return ecs->Get<Script>(e); });
	e_script->lua.set_function("LoadScript", [&](const std::string name, const std::string path) {e_script->LoadScript(name, path); });
	e_script->lua.set_function("Quit", [&]() {e->graphics->ShouldQuit(); });
	//Engine
	e_script->lua.set_function("KeyIsDown", [&](const int keycode) { return GetKeyDown(keycode); });
	//Sound
    e_script->LoadScript("CoinCollect", "CoinCollect");	
	std::string b = "CoinCollect";
	e_script->ScriptMap[b]();
	//e_my_function();
	
}

void engine::Engine::e_ShutDown()
{
	ecs->ForEach<Sprite>([&](EntityID e) {
		ecs->Destroy(e);
		});
	graphics->g_Shutdown();
	sound->Shutdown();
	std::cout << "Engine Shutdown"<< std::endl;
}

void engine::Engine::e_ReunGameLoop(const e_UpdateCallback& callback)
{
	int i = 0;
	int input_i = 0;
	std::cout << "Engine is running\n";

	double start_time = glfwGetTime();
	double dt = (1. / 60.);
	double accumulator = 0.0f;
    while (this->graphics->window_isRunning) {
		
		inputs->Update();
		glfwWaitEvents();
		//e_Update();
		callback();
		
		graphics->Draw(graphics->sprites);
		

		// Manage timestep
		double end_time = glfwGetTime();
		double frame_time = end_time - start_time;
		start_time = end_time;

		accumulator += frame_time;

		while (accumulator >= dt)
		{
			accumulator -= dt;
		}
		glfwPostEmptyEvent();
    }
}

void engine::Engine::HelloUser() {
	std::cout << "Hello User" << std::endl;
}
void engine::Engine::UserInput(std::shared_ptr<engine::Engine>& e) {
	
	if (glfwWindowShouldClose(e->graphics->window)) {
		std::cout << "User click on the exit box" << std::endl;
		e->graphics->ShouldQuit();
	}
	
	if (e->inputs->KeyIsPressed(e->graphics->window, GLFW_KEY_Q, GLFW_PRESS) || e->inputs->KeyIsPressed(e->graphics->window, GLFW_KEY_ESCAPE, GLFW_PRESS)) {
		std::cout << "User press Q or Escape to close Windows" << std::endl;
		e->graphics->ShouldQuit();
	}
	
	
	
}
void engine::Engine::EngineForEach()
{
	ecs->ForEach<Sprite>([&](EntityID entity) {
		Sprite& sprite = ecs->Get<Sprite>(entity);
		ecs->Get<Sprite>(entity).position = ecs->Get<Position>(entity);
		
		if (sprite.active && !sprite.in_active) {
			
			this->graphics->sprites.push_back(sprite);
			this->graphics->LoadImage(sprite.imageName, sprite.imagePath);
			ecs->Get<Sprite>(entity).in_active = true;
		}
		for (int m_s_i = 0; m_s_i < this->graphics->sprites.size(); m_s_i++) {
			if (sprite.imageName._Equal(this->graphics->sprites.at(m_s_i).imageName)) {
				this->graphics->sprites.at(m_s_i).position = sprite.position;

			}
		}
		if (!sprite.active && sprite.in_active) {
			for (int m_s_i = 0; m_s_i < this->graphics->sprites.size(); m_s_i++) {
				if (ecs->Get<Sprite>(entity).imageName._Equal(this->graphics->sprites.at(m_s_i).imageName)) {
					std::string a = this->graphics->sprites.at(m_s_i).imageName;
					this->graphics->g_tex[a].destoryit();
					this->graphics->g_tex.erase(a);
					this->graphics->sprites.erase(this->graphics->sprites.begin() + m_s_i);
				}
			}
			ecs->Get<Sprite>(entity).in_active = false;
		}
		}
	);
	ecs->ForEach<Sprite, Rigidbody, Position>([&](EntityID e) {
		Sprite s = ecs->Get<Sprite>(e);
		Position s_position = ecs->Get<Position>(e);
		Rigidbody s_rigidbody = ecs->Get<Rigidbody>(e);
		auto a = ecs->GetAppropriateSparseSet<Collider>();
		if (s.active && s_rigidbody.active) {
			s_position.x = s_position.x + s_rigidbody.velocity.x + DeltaTime;
			s_position.z = s_position.z + s_rigidbody.velocity.z + DeltaTime;
			s_position.y = (float)(0.5 * DeltaTime * DeltaTime * s_rigidbody.meters_per_second + DeltaTime * s_rigidbody.velocity.y + s_position.y);
			ecs->Get<Sprite>(e).position = s_position;
			ecs->Get<Position>(e) = s_position;
			if (a[e].active) {
				a[e].offset = s_position - a[e].offset;
			}
		}
		});
	ecs->ForEach<Sprite, Health>([&](EntityID e) {

		if (ecs->Get<Health>(e).percent < 0 && ecs->Get<Health>(e).percent != DBL_MIN) {
			for (int m_s_i = 0; m_s_i < this->graphics->sprites.size(); m_s_i++) {
				if (ecs->Get<Sprite>(e).imageName._Equal(this->graphics->sprites.at(m_s_i).imageName)) {
					std::string a = this->graphics->sprites.at(m_s_i).imageName;
					this->graphics->g_tex[a].destoryit();
					this->graphics->g_tex.erase(a);
					this->graphics->sprites.erase(this->graphics->sprites.begin() + m_s_i);
					std::cout << a << " is died" << std::endl;
				}
			}
			if (!ecs->Get<Script>(e).name.empty()) { e_script->ScriptMap.erase(ecs->Get<Script>(e).name); }
			ecs->Destroy(e);
		}

		});
	ecs->ForEach<Script>([&](EntityID entity) {
		if (!ecs->Get<Script>(entity).name.empty()) {

			std::string script_name = ecs->Get<Script>(entity).name;
			//std::cout << entity << std::endl;
			if (e_script->ScriptMap.count(script_name) == 0) {
				std::cout << script_name << std::endl;
				e_script->LoadScript(script_name, script_name);
			}
			sol::protected_function_result script2result = e_script->ScriptMap[script_name]();
			if (script2result.valid()) {
				//std::cout << script_name << std::endl;
			}
			else {
				sol::error err = script2result;
				std::string what = err.what();
				std::cout << "What is the problem: " << what << std::endl;
			}
		}
		}
	);
}
void engine::Engine::addScript(EntityID e, const std::string name)
{
	this->ecs->Get<Script>(e).name = name;
}
bool engine::Engine::GetKeyDown(int key) {
	bool old = this->inputs->KeyIsPressed(this->graphics->window, key, GLFW_PRESS);
	return old;

};
engine::Engine::Engine() {
	engine::Engine::graphics = std::make_shared<GraphicsManager>();
	engine::Engine::inputs = std::make_shared<InputManager>();
	engine::Engine::sound = std::make_shared<SoundManager>();
	engine::Engine::ecs = std::make_shared<ECS>();
	engine::Engine::e_script = std::make_shared<ScriptsManager>();
	engine::Engine::graphics->window_isRunning = true;
}
void engine::Engine::e_my_function()
{


}

void engine::Engine::e_Update()
{
	
	
}
