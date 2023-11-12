
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
	gui->Startup(this->graphics->window);
	graphics->g_StartUp();
	inputs->StartUp();
	sound->StartUp();
	e_script->StartUp();
	//ECS
	//Single Entities
	e_script->lua.set_function("CreateObject", [&](const std::string name, const std::string path) {ecs->Create(name, path); });
	e_script->lua.set_function("Destroy", [&](const EntityID e) {
		auto& container = ecs->GetAppropriateSparseSet<Sprite>();
		for (const auto& [entity, value] : container) {
			Sprite s = ecs->Get<Sprite>(entity);
			if (e == (s.EntityN)) {
				if (s.active) {
					for (int i = 0; i < this->graphics->sprites.size(); i++) {
						if (s.EntityN == (this->graphics->sprites.at(i).EntityN)) {
							std::string a = this->graphics->sprites.at(i).imageName;
							this->graphics->g_tex[a].destoryit();
							this->graphics->g_tex.erase(a);
							this->graphics->sprites.erase(this->graphics->sprites.begin() + i);
						}
					}
				}
				ecs->Destroy(entity);
			}
			EntityID tmp_i = 0;
			if (!ecs->environmentManipulation.empty()) {
				for (auto i = ecs->environmentManipulation.begin(); i != ecs->environmentManipulation.end(); i++) {
					if (ecs->environmentManipulation.at(tmp_i) == e) {
						ecs->environmentManipulation.erase(i);
					}
					tmp_i++;
				}
			}
			
		}
		});
	e_script->lua.set_function("GetSprite", [&](EntityID e) -> Sprite& { return ecs->Get<Sprite>(e); });
	e_script->lua.set_function("SetHealth", [&](EntityID e, double p) {ecs->Get<Health>(e).percent = p; });
	e_script->lua.set_function("SetPosition", [&](EntityID e, float x, float y, float z) {ecs->setPosition(e, m_Types::vec3(x, y, z)); });
	e_script->lua.set_function("AddForce", [&](EntityID e, float force) {
		ecs->Get<Rigidbody>(e).force.y = (float)(360 * ((9.8 + force * ecs->Get<Rigidbody>(e).f_weight)) / ecs->Get<Rigidbody>(e).mass); 
		ecs->Get<Rigidbody>(e).f_weight -= (float)(1.0f / 3600.0f);
	});
	e_script->lua.set_function("AddVelocity", [&](EntityID e, float x, float y, float z) {
		ecs->Get<Rigidbody>(e).velocity.x = x;
		ecs->Get<Rigidbody>(e).velocity.y = y; 
		ecs->Get<Rigidbody>(e).velocity.z = z;
		});
	e_script->lua.set_function("SetScale", [&](EntityID e, float x, float y, float z) {ecs->Get<Sprite>(e).scale.x = x; ecs->Get<Sprite>(e).scale.y += y; ecs->Get<Sprite>(e).scale.z = z; });
	e_script->lua.set_function("Collide", [&](EntityID e, std::string tag) {return ecs->Collide(e, tag); });
	e_script->lua.set_function("AddRigidBody", [&](EntityID e) {
		ecs->Get<Rigidbody>(e).active = true;
		ecs->Get<Rigidbody>(e).gravity = true;
		ecs->Get<Rigidbody>(e).mass = 1;
		ecs->Get<Rigidbody>(e).velocity = vec3(0,0,0);
		ecs->Get<Rigidbody>(e).force = vec3(0, 0, 0);
		ecs->Get<Rigidbody>(e).meters_per_second = -9.8 * 360;
		ecs->Get<Rigidbody>(e).f_weight = 1.0f;
		});
	e_script->lua.set_function("setActiveGravity", [&](EntityID e, bool activeCode) {
		ecs->Get<Rigidbody>(e).gravity = activeCode;
		});
	e_script->lua.set_function("AddScript", [&](EntityID e, const std::string name){addScript(e, name); });
	e_script->lua.set_function("GetScript", [&](EntityID e) -> Script& {return ecs->Get<Script>(e); });
	e_script->lua.set_function("LoadScript", [&](const std::string name, const std::string path) {e_script->LoadScript(name, path); });
	//Environment
	e_script->lua.set_function("ECreateObject", [&](const std::string name, const std::string path) {
		ecs->Create(name, path);
		addECSEnvironment(ecs->sizeEntity - 1);
		ecs->Get<Rigidbody>(ecs->sizeEntity - 1).active = true;
		ecs->Get<Rigidbody>(ecs->sizeEntity - 1).gravity = true;
		ecs->Get<Rigidbody>(ecs->sizeEntity - 1).mass = 1;
		ecs->Get<Rigidbody>(ecs->sizeEntity - 1).velocity = vec3(0, 0, 0);
		ecs->Get<Rigidbody>(ecs->sizeEntity - 1).force = vec3(0, 0, 0);
		ecs->Get<Rigidbody>(ecs->sizeEntity - 1).meters_per_second = +9.8 * 360;
		ecs->Get<Rigidbody>(ecs->sizeEntity - 1).f_weight = 1.0f;
		});
	e_script->lua.set_function("ESetPosition", [&](float x, float y, float z) {
		for (int i = 0; i < ecs->environmentManipulation.size(); i++) {
			ecs->setPosition(ecs->environmentManipulation.at(i), m_Types::vec3(x, y, z));
		}

	});
	e_script->lua.set_function("EAddForce", [&](float force) {
		for (int i = 0; i < ecs->environmentManipulation.size(); i++) {
			ecs->Get<Rigidbody>(ecs->environmentManipulation.at(i)).force.y = -1 * (float)(360 * ((9.8 + force * ecs->Get<Rigidbody>(ecs->environmentManipulation.at(i)).f_weight)) / ecs->Get<Rigidbody>(ecs->environmentManipulation.at(i)).mass);
			ecs->Get<Rigidbody>(ecs->environmentManipulation.at(i)).f_weight -= (float)(1.0f / 3600.0f);
		}
		
		});
	e_script->lua.set_function("EAddVelocity", [&](float x, float y, float z) {
		for (int i = 0; i < ecs->environmentManipulation.size(); i++) {
			ecs->Get<Rigidbody>(ecs->environmentManipulation.at(i)).velocity.x = -1*x;
			ecs->Get<Rigidbody>(ecs->environmentManipulation.at(i)).velocity.y = -1 * y;
			ecs->Get<Rigidbody>(ecs->environmentManipulation.at(i)).velocity.z = -1 * z;
		}
		
	});
	e_script->lua.set_function("ESetScale", [&](EntityID e, float x, float y, float z) {
		for (int i = 0; i < ecs->environmentManipulation.size(); i++) {
			ecs->Get<Sprite>(ecs->environmentManipulation.at(i)).scale.x = -1 * x;
			ecs->Get<Sprite>(ecs->environmentManipulation.at(i)).scale.y = -1 * y;
			ecs->Get<Sprite>(ecs->environmentManipulation.at(i)).scale.z = -1 * z;
		}
		
		});

	e_script->lua.set_function("EsetActiveGravity", [&](bool activeCode) {
		for (int i = 0; i < ecs->environmentManipulation.size(); i++) {
			ecs->Get<Rigidbody>(ecs->environmentManipulation.at(i)).gravity = activeCode;
		}
		
		});
	//Engine
	e_script->lua.set_function("KeyIsDown", [&](const int keycode) { return GetKeyDown(keycode); });
	e_script->lua.set_function("Quit", [&]() {e->graphics->ShouldQuit(); });
	//Sound
    e_script->LoadScript("CoinCollect", "CoinCollect");	
	std::string b = "CoinCollect";
	e_script->ScriptMap[b]();
	//e_my_function();
	
}
void engine::Engine::addECSEnvironment(EntityID e) {
	this->ecs->environmentManipulation.push_back(e);
}
void engine::Engine::e_ShutDown()
{
	ecs->ForEach<Sprite>([&](EntityID e) {
		ecs->Destroy(e);
		});
	graphics->g_Shutdown();
	sound->Shutdown();
	e_script->Shutdown();
	gui->Shutdown();
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
		//glfwWaitEvents();
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
			if (sprite.EntityN == (this->graphics->sprites.at(m_s_i).EntityN)) {
				this->graphics->sprites.at(m_s_i).position = sprite.position;

			}
		}
		if (!sprite.active && sprite.in_active) {
			for (int m_s_i = 0; m_s_i < this->graphics->sprites.size(); m_s_i++) {
				if (ecs->Get<Sprite>(entity).EntityN == (this->graphics->sprites.at(m_s_i).EntityN)) {
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

	int64_t m_test = 0;
	u_result = false;
	bool result = false;
	ecs->ForEach<Sprite, BoxCollider>([&](EntityID e) {
		Sprite s = ecs->Get<Sprite>(e);
		ecs->ForEach< Sprite, BoxCollider>([&](EntityID e1) {
			if (s.EntityN != ecs->Get<Sprite>(e1).EntityN && m_test <= e1) {

				result = ecs->BoxCollide(e, e1);
				if (!u_result) {
					u_result = result;
				}

				std::cout << "Problem: " << u_result << std::endl;
			}
		});
		m_test++;
	});
	for (int i = 0; i < this->ecs->environmentManipulation.size(); i++) {
		EntityID e = this->ecs->environmentManipulation.at(i);
		Sprite s = ecs->Get<Sprite>(e);
		m_Types::vec3 s_position;
		Rigidbody s_rigidbody = ecs->Get<Rigidbody>(e);
		if (this->inputs->KeyIsPressed(this->graphics->window, GLFW_KEY_W, GLFW_RELEASE)) { ecs->Get<Rigidbody>(e).f_weight = 1.0f; }

		if (s.active && s_rigidbody.active && s_rigidbody.velocity.y == 0) {
			s_position.x = s_rigidbody.velocity.x * DeltaTime;
			s_position.z = s_rigidbody.velocity.z * DeltaTime;
			s_position.y = (float)(DeltaTime * s_rigidbody.velocity.y + (DeltaTime * DeltaTime * s_rigidbody.force.y * 0.5));
			if (s_rigidbody.gravity && !u_result) {
				s_position.y += (float)(0.5 * s_rigidbody.meters_per_second * DeltaTime * DeltaTime);
			}
			ecs->setPosition(e, s_position);
		}
		ecs->ForEach< Sprite, BoxCollider>([&](EntityID e1) {
			if (s.EntityN != ecs->Get<Sprite>(e1).EntityN && m_test <= e1) {
				result = ecs->BoxCollide(e, e1);
						//std::cout << "Problem: " << result << std::endl;
			}
		});
	}
	ecs->ForEach<Sprite, BoxCollider, Rigidbody>(([&](EntityID e){
		bool background = false;
		for (int i = 0; i < ecs->environmentManipulation.size(); i++) {
			if (e == ecs->environmentManipulation.at(i)) {
				background = true;
			}
		}
		if (!background) {
			Sprite s = ecs->Get<Sprite>(e);
			m_Types::vec3 s_position;
			Rigidbody s_rigidbody = ecs->Get<Rigidbody>(e);
			if (this->inputs->KeyIsPressed(this->graphics->window, GLFW_KEY_W, GLFW_RELEASE)) { ecs->Get<Rigidbody>(e).f_weight = 1.0f; }

			if (s.active && s_rigidbody.active && s_rigidbody.velocity.y == 0) {
				s_position.x = s_rigidbody.velocity.x * DeltaTime;
				s_position.z = s_rigidbody.velocity.z * DeltaTime;
				s_position.y = (float)(DeltaTime * s_rigidbody.velocity.y + (DeltaTime * DeltaTime * s_rigidbody.force.y * 0.5));
				if (s_rigidbody.gravity && !u_result) {
					s_position.y += (float)(0.5 * s_rigidbody.meters_per_second * DeltaTime * DeltaTime);
				}
				ecs->setPosition(e, s_position);
			}
			ecs->ForEach< Sprite, BoxCollider>([&](EntityID e1) {
				if (s.EntityN != ecs->Get<Sprite>(e1).EntityN && m_test <= e1) {
					result = ecs->BoxCollide(e, e1);
					//std::cout << "Problem: " << result << std::endl;
				}
				});
		}
		
	}));
	ecs->ForEach<Sprite, Rigidbody>([&](EntityID e) {
		//Deacceleration
		if (ecs->Get<Rigidbody>(e).force.y - 60 < 0) {
			ecs->Get<Rigidbody>(e).force.y = 0;
		}
		else {
			ecs->Get<Rigidbody>(e).force.y -= 60;
		}
		if (ecs->Get<Rigidbody>(e).velocity.x - 0.1 < 0) {
			ecs->Get<Rigidbody>(e).velocity.x = 0;
		}
		else {
			ecs->Get<Rigidbody>(e).velocity.x -= 0.1;
		}
		if (ecs->Get<Rigidbody>(e).velocity.y - 0.1 < 0) {
			ecs->Get<Rigidbody>(e).velocity.y = 0;
		}
		else {
			ecs->Get<Rigidbody>(e).velocity.y -= 0.1;
		}
	});

	ecs->ForEach<Sprite, Health>([&](EntityID e) {

		if (ecs->Get<Health>(e).percent < 0 && ecs->Get<Health>(e).percent != DBL_MIN) {
			for (int m_s_i = 0; m_s_i < this->graphics->sprites.size(); m_s_i++) {
				if (ecs->Get<Sprite>(e).EntityN == (this->graphics->sprites.at(m_s_i).EntityN)) {
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
	gui = std::make_shared<GUIManager>();
	engine::Engine::graphics->window_isRunning = true;
}
void engine::Engine::e_my_function()
{


}

void engine::Engine::e_Update()
{
	
	
}
