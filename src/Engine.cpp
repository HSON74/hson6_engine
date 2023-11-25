
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
	engine::Engine::graphics = std::make_shared<GraphicsManager>();
	engine::Engine::inputs = std::make_shared<InputManager>();
	engine::Engine::sound = std::make_shared<SoundManager>();
	engine::Engine::ecs = std::make_shared<ECS>();
	engine::Engine::e_script = std::make_shared<ScriptsManager>();
	gui = std::make_shared<GUIManager>();
	animation = std::make_shared<AnimatorManager>();
	cam = std::make_shared<Camera>();
	cam->active = false;
	cam->fovy = 60;
	cam->aspect = (double)((double)(graphics->Width) / (double)(graphics->Height));
	cam->c_far = 1.00f;
	cam->c_near = 0.03f;
	cam->r_far = 1000.0f;
	cam->r_near = 0.03f;
	e_perp(this->cam);
	engine::Engine::graphics->window_isRunning = true;
	graphics->g_StartUp();
	inputs->StartUp();
	//gui->Startup();
	sound->StartUp();
	e_script->StartUp();
	//ECS
	//Single Entities
	e_script->lua.set_function("CreateObject", [&](const std::string name, const std::string path) {ecs->Create(name, path); });
	e_script->lua.set_function("CreateUI", [&](const std::string name, const std::string path) {
		ecs->CreateUI(name); 
	});
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
		ecs->Get<Rigidbody>(e).force.y = (float)(360 * (((9.8 + force) * 5 * ecs->Get<Rigidbody>(e).f_weight)) / ecs->Get<Rigidbody>(e).mass);
		ecs->Get<Rigidbody>(e).f_weight -= (float)(1.0f / 3600.0f);
	});
	e_script->lua.set_function("AddVelocity", [&](EntityID e, float x, float y, float z) {
		ecs->Get<Rigidbody>(e).velocity.x = x * 5;
		ecs->Get<Rigidbody>(e).velocity.y = y * 5;
		ecs->Get<Rigidbody>(e).velocity.z = z * 5;
		});
	e_script->lua.set_function("SetScale", [&](EntityID e, float x, float y, float z) {
		ecs->Get<Sprite>(e).scale.x = x; 
		ecs->Get<Sprite>(e).scale.y = y; 
		ecs->Get<Sprite>(e).scale.z = z; 
		ecs->Get<BoxCollider>(e).size.x = std::abs(x) * 100.f;
		ecs->Get<BoxCollider>(e).size.y = std::abs(y) * 100.f;
		ecs->Get<BoxCollider>(e).size.z = std::abs(z) * 100.f;
		});
	e_script->lua.set_function("CoinCollide", [&](EntityID e, std::string tag) {return ecs->BoxEntity(e, tag); });
	e_script->lua.set_function("Collide", [&](EntityID e, std::string tag) {return ecs->Collide(e, tag); });
	e_script->lua.set_function("CheckCollide", [&](EntityID e, EntityID e1) {return ecs->CheckBoxCollide(e, e1); });
	e_script->lua.set_function("AddRigidBody", [&](EntityID e) {
		ecs->Get<Rigidbody>(e).active = true;
		ecs->Get<Rigidbody>(e).gravity = true;
		ecs->Get<Rigidbody>(e).mass = 1;
		ecs->Get<Rigidbody>(e).velocity = vec3(0,0,0);
		ecs->Get<Rigidbody>(e).force = vec3(0, 0, 0);
		ecs->Get<Rigidbody>(e).meters_per_second = -9.8 * 360 * 5;
		ecs->Get<Rigidbody>(e).f_weight = 1.0f;
		ecs->Get<Rigidbody>(e).canToggle = false;
		ecs->Get<Rigidbody>(e).toggle = 0.0f;

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
		ecs->Get<Rigidbody>(ecs->sizeEntity - 1).meters_per_second = (9.8 * 360 * 5);
		ecs->Get<Rigidbody>(ecs->sizeEntity - 1).f_weight = 1.0f;
		ecs->Get<BoxCollider>(ecs->sizeEntity - 1).IsStatis = false;
		});
	e_script->lua.set_function("setActiveTrigger", [&](EntityID e, bool activeCode) {
		ecs->Get<BoxCollider>(e).IsTrigger = activeCode;
		});
	e_script->lua.set_function("ESetPosition", [&](float x, float y, float z) {
		for (int i = 0; i < ecs->environmentManipulation.size(); i++) {
			ecs->setPosition(ecs->environmentManipulation.at(i), m_Types::vec3(x, y, z));
			
		}

	});
	e_script->lua.set_function("EAddForce", [&](float force) {
		for (int i = 0; i < ecs->environmentManipulation.size(); i++) {
			ecs->Get<Rigidbody>(ecs->environmentManipulation.at(i)).force.y = -1 * (float)(360 * (((9.8 + force) * 5 * ecs->Get<Rigidbody>(ecs->environmentManipulation.at(i)).f_weight)) / ecs->Get<Rigidbody>(ecs->environmentManipulation.at(i)).mass);
			ecs->Get<Rigidbody>(ecs->environmentManipulation.at(i)).f_weight -= (float)(1.0f / 3600.0f);
		}
		
		});
	e_script->lua.set_function("EAddVelocity", [&](float x, float y, float z) {
		for (int i = 0; i < ecs->environmentManipulation.size(); i++) {
			ecs->Get<Rigidbody>(ecs->environmentManipulation.at(i)).velocity.x = -1*x * 5;
			ecs->Get<Rigidbody>(ecs->environmentManipulation.at(i)).velocity.y = -1 * y * 5;
			ecs->Get<Rigidbody>(ecs->environmentManipulation.at(i)).velocity.z = -1 * z * 5;

		}
		
	});
	e_script->lua.set_function("ESetScale", [&](EntityID e, float x, float y, float z) {
		for (int i = 0; i < ecs->environmentManipulation.size(); i++) {
			ecs->Get<Sprite>(ecs->environmentManipulation.at(i)).scale.x = -1 * x;
			ecs->Get<Sprite>(ecs->environmentManipulation.at(i)).scale.y = -1 * y;
			ecs->Get<Sprite>(ecs->environmentManipulation.at(i)).scale.z = -1 * z;
			ecs->Get<BoxCollider>(ecs->environmentManipulation.at(i)).size.x = -x * 0.01f;
			ecs->Get<BoxCollider>(ecs->environmentManipulation.at(i)).size.y = -y * 0.01f;
			ecs->Get<BoxCollider>(ecs->environmentManipulation.at(i)).size.z = -z * 0.01f;
		}
		
		});

	e_script->lua.set_function("EsetActiveGravity", [&](bool activeCode) {
		for (int i = 0; i < ecs->environmentManipulation.size(); i++) {
			ecs->Get<Rigidbody>(ecs->environmentManipulation.at(i)).gravity = activeCode;
		}
		
		});
	e_script->lua.set_function("CreateUI", [&](std::string name) {
		ecs->CreateUI(name);
		ecs->Get<Sprite>(ecs->sizeEntity - 1).tag = "UI";
		});
	e_script->lua.set_function("UIUpdate", [&](EntityID e, std::string name) {
		ecs->ChangeText(e, name);
		});
	//Engine
	e_script->lua.set_function("KeyIsDown", [&](const int keycode) { return GetKeyDown(keycode); });
	e_script->lua.set_function("Quit", [&]() {e->graphics->ShouldQuit(); });
	//Sound
	e_script->lua.set_function("LoadSound", [&](std::string m_name, std::string m_path) {return e->sound->LoadSound(m_name, m_path); });
	e_script->lua.set_function("Play", [&](std::string m_name) {e->sound->Play(m_name); });
	e_script->lua.set_function("Loop", [&](std::string m_name, bool activecode) {e->sound->Loop(m_name, activecode); });
	e_script->lua.set_function("Stop", [&](std::string m_name) {e->sound->Stop(m_name); });
	e_script->lua.set_function("SetVolume", [&](std::string m_name, float vol) {e->sound->SetVolume(m_name, vol); });

	//Animation
	e_script->lua.set_function("CreateAnimator", [&](EntityID eid, std::string m_name, std::string m_path) {
		Animation tmp_animator;
		tmp_animator.animation_name = m_name;
		tmp_animator.speed = 1.0f;
		ecs->Get<EntityAnimator>(eid).e_Animator_Frame.push_back(tmp_animator);
		e->animation->CreateAnimator(eid, ecs->Get<EntityAnimator>(eid), m_name, m_path);
	});
	e_script->lua.set_function("CreateAnimatorIdle", [&](EntityID eid, std::string m_name, std::string m_path) {
		Animation tmp_animator;
		tmp_animator.animation_name = m_name;
		tmp_animator.speed = 1.0f;
		ecs->Get<EntityAnimator>(eid).e_Animator_Frame.push_back(tmp_animator);
		e->animation->CreateAnimatorIdle(eid, ecs->Get<EntityAnimator>(eid), m_name, m_path);
		});
	e_script->lua.set_function("AddAnimation", [&](EntityID eid, std::string m_name, std::string m_path) {
		e->animation->AddAnimation(ecs->Get<EntityAnimator>(eid), m_name, m_path);
		});
	e_script->lua.set_function("AddFrame", [&](EntityID eid, std::string m_name, std::string m_path) {
		e->animation->AddFrame(ecs->Get<EntityAnimator>(eid), m_name, m_path);
		});
	e_script->lua.set_function("PlayAnimation", [&](EntityID eid, std::string name) {
		for (int i = 0; i < e->animation->EntityWithAnimator.size(); i++) {
			if (eid == (EntityID)e->animation->EntityWithAnimator.at(i)) {
				e->animation->PlayAnimator(e->ecs->Get<EntityAnimator>(eid), name);
				break;
			}
		}
	});
	e_script->lua.set_function("StopAnimation", [&](EntityID eid, bool activecode) {
		for (int i = 0; i < e->animation->EntityWithAnimator.size(); i++) {
			if (eid == (EntityID)e->animation->EntityWithAnimator.at(i)) {
				e->animation->StopAnimator(e->ecs->Get<EntityAnimator>(eid), activecode);
				break;
			}
		}
		});
	e_script->lua.set_function("LoopAnimation", [&](EntityID eid, std::string name, bool activecode) {
		for (int i = 0; i < e->animation->EntityWithAnimator.size(); i++) {
			if (eid == (EntityID)e->animation->EntityWithAnimator.at(i)) {
				e->animation->LoopAnimator(e->ecs->Get<EntityAnimator>(eid), activecode, name);
				break;
			}
		}
		});
	e_script->lua.set_function("SetSpeedAnimation", [&](EntityID eid, std::string name, float speed) {
		for (int i = 0; i < e->animation->EntityWithAnimator.size(); i++) {
			if (eid == (EntityID)e->animation->EntityWithAnimator.at(i)) {
				e->animation->SetSpeed(e->ecs->Get<EntityAnimator>(eid), name, speed);
				break;
			}
		}
		});


	if (is_Test) {
		e_script->LoadScript("Test", "Test");
		std::string b = "Test";
		e_script->ScriptMap[b]();
	}
	else {
		e_script->LoadScript("CoinCollect", "CoinCollect");
		std::string b = "CoinCollect";
		e_script->ScriptMap[b]();
	}
  
	e_my_function();
	
}
void engine::Engine::addECSEnvironment(EntityID e) {
	this->ecs->environmentManipulation.push_back(e);
}
void engine::Engine::e_ShutDown()
{
	if (this->animation->EntityWithAnimator.size() != 0) {
		for (int i = 0; i < this->animation->EntityWithAnimator.size(); i++) {
			for (int j = 0; j < this->ecs->Get<EntityAnimator>(i).e_Animator_Frame.size(); j++) {
				this->ecs->Get<EntityAnimator>(i).e_Animator_Frame.at(j).destoryit();
			}
			this->ecs->Get<EntityAnimator>(i).destoryit();
		}
		this->animation->EntityWithAnimator.clear();
	}
	this->animation->shutdown();

	ecs->Shutdown();
	graphics->g_Shutdown();
	sound->Shutdown();
	e_script->Shutdown();
	//gui->Shutdown();
	std::cout << "Engine Shutdown"<< std::endl;
}

void engine::Engine::e_perp(std::shared_ptr<Camera>& cam)
{
	double tmp_fovy = (double)((cam->fovy * 3.14) / 180.00);

	cam->top = (float) (cam->c_near * std::tanf((float)tmp_fovy / 2.0f));
	cam->bottom = (float) (-1.00f*cam->top);
	cam->right = (float)((float)cam->aspect * cam->top);
	cam->left = (float)(- 1.00f * cam->right);
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
		if (graphics->Height > graphics->Width) {
			this->cam->aspect = (double)((float)(this->graphics->Height)/ (float) (this->graphics->Width));
			this->cam->c_near = (float)(this->graphics->Height * this->cam->r_near);
			this->cam->c_far = (float)(this->graphics->Height * this->cam->r_far);
		}
		else {
			this->cam->aspect = (double)((float)(this->graphics->Width)/ (float)(this->graphics->Height));
			this->cam->c_near = (float)(this->graphics->Width * this->cam->r_near);
			this->cam->c_far = (float)(this->graphics->Width * this->cam->r_far);
		}
		e_perp(this->cam);
		e_Update();
		callback();
		if (this->GetKeyDown(GLFW_KEY_P)) {
			isPlay = !isPlay;
		}
		this->EngineForEach();
		graphics->Draw(cam, graphics->sprites, graphics->UI_sprites);
		
		

		// Manage timestep
		double end_time = glfwGetTime();
		double frame_time = end_time - start_time;
		start_time = end_time;

		accumulator += frame_time;

		while (accumulator >= dt)
		{
			accumulator -= dt;
		}
		if (this->GetKeyDown(GLFW_KEY_APOSTROPHE)) {
			this->Reboot = true;
			this->graphics->window_isRunning = false;

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
			
			if (sprite.tag == "UI") {
				this->graphics->UI_sprites.push_back(this->ecs->Get<UI>(sprite.EntityN));
				sprite.UI_location = (int)this->graphics->UI_sprites.size() - 1;
			}
			else {
				this->graphics->LoadImage(sprite.imageName, sprite.imagePath);
			}
			this->graphics->sprites.push_back(sprite);
			ecs->Get<Sprite>(entity).in_active = true;
		}
		for (int m_s_i = 0; m_s_i < this->graphics->sprites.size(); m_s_i++) {
			if (sprite.EntityN == (this->graphics->sprites.at(m_s_i).EntityN)) {
				this->graphics->sprites.at(m_s_i).position = sprite.position;
				this->graphics->sprites.at(m_s_i).scale = sprite.scale;
				this->graphics->sprites.at(m_s_i).imagePath = sprite.imagePath;
				//this->graphics->sprites.at(m_s_i).active = sprite.active;
				//this->graphics->sprites.at(m_s_i).in_active = sprite.in_active;
				//this->graphics->sprites.at(m_s_i).imageName = sprite.imagePath;
				//this->graphics->sprites.at(m_s_i).rotation = sprite.rotation;
				//this->graphics->sprites.at(m_s_i).size = sprite.size;
				//this->graphics->sprites.at(m_s_i).tag = sprite.tag;
				//if (sprite.tag == "UI" || sprite.tag == "Background") {
					//this->graphics->UI_sprites.push_back(this->ecs->Get<UI>(sprite.EntityN));
					//sprite.UI_location = (int)this->graphics->UI_sprites.size() - 1;
				//}
				//this->graphics->sprites.at(m_s_i).UI_location = sprite.UI_location;
				//this->graphics->sprites.at(m_s_i).z_value = sprite.z_value;


			}
		}
		if (!sprite.active && sprite.in_active) {
			if(sprite.tag == "UI") {
				//Do somthing
			}
			else {
				for (int m_s_i = 0; m_s_i < this->graphics->sprites.size(); m_s_i++) {
					if (ecs->Get<Sprite>(entity).EntityN == (this->graphics->sprites.at(m_s_i).EntityN)) {
						std::string a = this->graphics->sprites.at(m_s_i).imageName;
						this->graphics->g_tex[a].destoryit();
						this->graphics->g_tex.erase(a);
						this->graphics->sprites.erase(this->graphics->sprites.begin() + m_s_i);
					}
				}
			}
			ecs->Get<Sprite>(entity).in_active = false;
		}
		}
	);
	if (this->isPlay) {
		int64_t m_test = 0;
		u_result = false;
		bool result = false;
		EntityID tmp1 = -1;
		this->ecs->ForEach<Sprite>([&](EntityID e) {
			u_result = false;
			
			for (int i = 0; i < this->ecs->environmentManipulation.size(); i++) {
				if (e == this->ecs->environmentManipulation.at(i)) {
					result = true;
				}
			}
			if (!result) {
				if (this->ecs->Get<Sprite>(e).tag == "UI" || this->ecs->Get<Sprite>(e).tag == "Background") {
					//std::cout << "Entity: " << this->ecs->Get<Sprite>(e).EntityN << " tag: " << this->ecs->Get<Sprite>(e).tag << std::endl;
					return;
				}
				for (int i = 0; i < this->ecs->environmentManipulation.size(); i++) {
					if (!u_result) {
						u_result = this->ecs->CheckBoxCollide(e, this->ecs->environmentManipulation.at(i));
					}
				}

			}
			m_Types::vec3 m_tmp = m_Types::vec3(0, 0, 0);
			if (u_result) {
				for (int i = 0; i < this->ecs->environmentManipulation.size(); i++) {
					EntityID e1 = this->ecs->environmentManipulation.at(i);
					float m_gravity = 360 * ((9.8f * 5*ecs->Get<Rigidbody>(e1).f_weight)) / ecs->Get<Rigidbody>(e1).mass;
					m_Types::vec3 s_position;
					Rigidbody s_rigidbody = this->ecs->Get<Rigidbody>(e1);
					s_position.x = s_rigidbody.velocity.x * DeltaTime;
					s_position.z = s_rigidbody.velocity.z * DeltaTime;
					s_position.y = (float)(DeltaTime * s_rigidbody.velocity.y + (DeltaTime * DeltaTime * (s_rigidbody.force.y - m_gravity) * 0.5));
					this->ecs->setPosition(e1, s_position);
					if (this->ecs->Get<Rigidbody>(this->ecs->environmentManipulation.at(i)).velocity.x < 0 && isFlipLeft) {
						isFlipLeft = false;
						//std::cout << "It fliped is false" << std::endl;
						tmp1 = -1;
					}
					else if (this->ecs->Get<Rigidbody>(this->ecs->environmentManipulation.at(i)).velocity.x > 0 && !isFlipLeft) {
						isFlipLeft = true;
						//std::cout << "It fliped is true" << std::endl;
						tmp1 = -1;
					}
				}
				for (int i = 0; i < this->ecs->environmentManipulation.size(); i++) {
					if(this->ecs->Get<Sprite>(this->ecs->environmentManipulation.at(i)).tag == "UI" || this->ecs->Get<Sprite>(this->ecs->environmentManipulation.at(i)).tag == "Background") {
						continue;
					}
					m_Types::vec3 c = this->ecs->Get<BoxCollider>(e).offset;
					m_Types::vec2 c_size = this->ecs->Get<BoxCollider>(e).size;
					m_Types::vec3 d = this->ecs->Get<BoxCollider>(this->ecs->environmentManipulation.at(i)).offset;
					m_Types::vec2 d_size = this->ecs->Get<BoxCollider>(this->ecs->environmentManipulation.at(i)).size;
					if (this->ecs->Get<BoxCollider>(this->ecs->environmentManipulation.at(i)).IsTrigger) {
						continue;
					}
					m_Types::vec3 tmp = m_Types::vec3(0, 0, 0);
					bool s_check = false;
					EntityID tmp2 = -1;
					if (((c.x <= d.x && c.x + c_size.x >= d.x - d_size.x || c.x >= d.x && c.x - c_size.x <= d.x + d_size.x)) &&
						((c.y >= d.y && c.y - c_size.y <= d.y + d_size.y) || (c.y <= d.y && c.y + c_size.y >= d.y - d_size.y))) {
						m_Types::vec3 tmptmp = m_Types::vec3(0, 0, 0);
						s_check = true;
						tmp2 = this -> ecs->environmentManipulation.at(i);
						if (c.x <= d.x && c.x + c_size.x >= d.x - d_size.x) {
							tmptmp.x += ((c.x + c_size.x) - (d.x - d_size.x));
						}
						else if (c.x >= d.x && c.x - c_size.x <= d.x + d_size.x) {
							tmptmp.x += ((c.x - c_size.x) - (d.x + d_size.x));
						}
						if (c.y >= d.y && c.y - c_size.y <= d.y + d_size.y) {
							tmptmp.y += ((c.y - c_size.y) - (d.y + d_size.y));
						}
						else if (c.y <= d.y && c.y + c_size.y >= d.y - d_size.y) {
							tmptmp.y += ((c.y + c_size.y) - (d.y - d_size.y));
						}
						if(std::abs(tmptmp.x) == std::abs(tmptmp.y)){
							tmp.x = tmptmp.x;
							tmp.y = tmptmp.y;
						}
						else if (std::abs(tmptmp.x) < std::abs(tmptmp.y)) {
							tmp.x = tmptmp.x;
						}
						else if (std::abs(tmptmp.x) > std::abs(tmptmp.y)) {
							tmp.y = tmptmp.y;
							
						}
						if ((this->ecs->Get<BoxCollider>(this->ecs->environmentManipulation.at(i)).IsTrigger)) {
							tmp = m_Types::vec3(0, 0, 0);
						}
					}
					if (s_check) {
						if (tmp1 != tmp2 && tmp2 != -1) {
							if (tmp1 != -1) {
								if (this->ecs->CheckBoxCollide(tmp1, tmp2)) {
									BoxCollider s_Object_a = this->ecs->Get<BoxCollider>(tmp1);
									BoxCollider s_Object_b = this->ecs->Get<BoxCollider>(tmp2);
									if (s_Object_a.offset.y == s_Object_b.offset.y) {
										//std::cout << "x velocity: " << (-1*this->ecs->Get<Rigidbody>(this->ecs->environmentManipulation.at(i)).velocity.x) << std::endl;
										//std::cout << "x tmp : "<< tmp.x << std::endl;
										if (this->ecs->Get<Rigidbody>(this->ecs->environmentManipulation.at(i)).velocity.x < 0 
											&& tmp.x > 0 && !isFlipLeft) {
											//std::cout << "Move Right" << std::endl;
											m_tmp.x = -1 * tmp.x;;
										}
										else if (this->ecs->Get<Rigidbody>(this->ecs->environmentManipulation.at(i)).velocity.x > 0 
											&& tmp.x < 0 && isFlipLeft) {
											m_tmp.x = -1* tmp.x;
											//std::cout << "Move Left" << std::endl;
										}
										else {
											m_tmp.x = tmp.x;
										}

										if ((std::abs(m_tmp.y) > std::abs(tmp.y) && tmp.y != 0) || m_tmp.y == 0) {
											m_tmp.y = tmp.y;
										}
									}
									else {
										if ((std::abs(m_tmp.x) > std::abs(tmp.x) && tmp.x != 0) || m_tmp.x == 0) {
											m_tmp.x = tmp.x;
										}
										if ((std::abs(m_tmp.y) > std::abs(tmp.y) && tmp.y != 0) || m_tmp.y == 0) {
											//m_tmp.y = tmp.y;
											m_tmp.y = tmp.y;
										}
									}
								}
							}
							else {
								tmp1 = tmp2;
								if ((std::abs(m_tmp.x) > std::abs(tmp.x) && tmp.x != 0) || m_tmp.x == 0) {
									m_tmp.x = tmp.x;
								}
								if ((std::abs(m_tmp.y) > std::abs(tmp.y) && tmp.y != 0) || m_tmp.y == 0) {
									//m_tmp.y = tmp.y;
									m_tmp.y = tmp.y;
								}
							}
							
						}
					}
				}
				
				for (int i = 0; i < this->ecs->environmentManipulation.size(); i++) {
					bool s_check = false;
					if (this->ecs->Get<Sprite>(this->ecs->environmentManipulation.at(i)).tag == "UI" || this->ecs->Get<Sprite>(this->ecs->environmentManipulation.at(i)).tag == "Background") {
						s_check = false;
					}
					if (!s_check) {
						
						this->ecs->setPosition(this->ecs->environmentManipulation.at(i), m_tmp);
					}
					
				}
				m_tmp = m_Types::vec3(0, 0, 0);
			}
			else if (result){
				tmp1 = -1;
				for (int i = 0; i < this->ecs->environmentManipulation.size(); i++) {
					EntityID e1 = this->ecs->environmentManipulation.at(i);
					m_Types::vec3 s_position;
					Rigidbody s_rigidbody = this->ecs->Get<Rigidbody>(e1);
					s_position.x = s_rigidbody.velocity.x * DeltaTime;
					s_position.z = s_rigidbody.velocity.z * DeltaTime;
					s_position.y = (float)(DeltaTime * s_rigidbody.velocity.y + (DeltaTime * DeltaTime * s_rigidbody.force.y * 0.5));
					if (s_rigidbody.gravity) {
						s_position.y += (float)(0.5 * s_rigidbody.meters_per_second * DeltaTime * DeltaTime);
					}
					this->ecs->setPosition(e1, s_position);
				}

			}
			m_test++;
			m_tmp = m_Types::vec3(0, 0, 0);
			result = false;
			if (this->inputs->KeyIsPressed(this->graphics->window, GLFW_KEY_W, GLFW_RELEASE) || u_result) { ecs->Get<Rigidbody>(e).f_weight = 1.0f; }
		});

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
				EntityID c_auto = 0;
				for (auto a_auto = this->animation->EntityWithAnimator.begin(); a_auto != this->animation->EntityWithAnimator.end();  a_auto++) {
					if (e == this->animation->EntityWithAnimator.at(c_auto)) {
						if (this->ecs->Get<EntityAnimator>(c_auto).e_Animator_Frame.size() != 0) {
							for (int j = 0; j < this->ecs->Get<EntityAnimator>(c_auto).e_Animator_Frame.size(); j++) {
								this->ecs->Get<EntityAnimator>(c_auto).e_Animator_Frame.at(j).destoryit();
							}
							this->ecs->Get<EntityAnimator>(c_auto).destoryit();
						}
						this->animation->EntityWithAnimator.erase(a_auto);
						break;
					}
					c_auto++;
				}
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
		});
		if (this->animation->EntityWithAnimator.size() != 0) {
			for (int i = 0; i < this->animation->EntityWithAnimator.size(); i++) {
				this->animation->NextFrame(this->ecs->Get<EntityAnimator>(this->animation->EntityWithAnimator.at(i)));
				Sprite c_sprite = this->ecs->Get<Sprite>(this->animation->EntityWithAnimator.at(i));
				EntityAnimator c_animation = this->ecs->Get<EntityAnimator>(this->animation->EntityWithAnimator.at(i));
				this->graphics->g_tex[c_sprite.imageName].destoryit();
				this->graphics->LoadFrame(c_sprite.imageName, c_animation.path);
				this->ecs->Get<Sprite>(this->animation->EntityWithAnimator.at(i)).imagePath = c_animation.path;
			}
		}
		
	}
}

void engine::Engine::addScript(EntityID e, const std::string name)
{
	this->ecs->Get<Script>(e).name = name;
};
bool engine::Engine::GetKeyDown(int key) {
	bool old = this->inputs->KeyIsPressed(this->graphics->window, key, GLFW_PRESS);
	return old;

};
engine::Engine::Engine() {
	
}
void engine::Engine::e_my_function()
{
	if (is_Test) {
		jump_count = 0;
		this->graphics->changeBackground(0, 0, 1);
		isJump = false;
	}
	else {
		//ecs->Create("red_box", "Red_Box.png");
		e_script->LoadScript("GameWorld", "GameWorld");
		std::string b = "GameWorld";
		e_script->ScriptMap[b]();
	}

}

void engine::Engine::e_Update()
{
	if (is_Test) {
		
		if (isPlay) {
			if (this->ecs->CheckBoxCollide(0, 6) && !isJump) {
				jump_count++;
				isJump = true;
			}
			else if(!(this->ecs->CheckBoxCollide(0, 6)) && isJump){
				isJump = false;
				std::cout << jump_count << std::endl;
			}
			int tmp_num = jump_count % 10;
			this->ecs->ChangeText(5, std::to_string(tmp_num));
			this->graphics->UI_sprites.at(this->ecs->Get<Sprite>(5).UI_location) = this->ecs->Get<UI>(5);
			tmp_num = jump_count / 100;
			this->ecs->ChangeText(3, std::to_string(tmp_num));
			this->graphics->UI_sprites.at(this->ecs->Get<Sprite>(3).UI_location) = this->ecs->Get<UI>(3);
			tmp_num = jump_count - ((jump_count / 100) * 100 + (jump_count % 10));
			tmp_num = tmp_num / 10;
			this->ecs->ChangeText(4, std::to_string(tmp_num));
			this->graphics->UI_sprites.at(this->ecs->Get<Sprite>(4).UI_location) = this->ecs->Get<UI>(4);
		}
	}
	else {

	}
}
/*
	public void Camera( float E, float e, float M,float m) {
		  float tiltAngle = 45; 

		    //1. camera faces the positive x axis
		    Rotatef(-90*dg, 0, 1, 0);

		    //2. camera on positive x axis
		    Translatef(-M, 0, 0);

		    //3. camera rotates around y axis (with the moon) 
		    Rotatef(-m, 0.0f, 1.0f, 0.0f);

		    // and so on reversing the solar transformation
		    Translatef(0, -E, 0);
		    Rotatef(-tiltAngle*dg, 0, 0, 1); // tilt angle
		    // rotating around the "sun"; proceed angle
		    Rotatef(-e, 0, 1, 0);
		    
		    // and reversing the robot transformation*/
