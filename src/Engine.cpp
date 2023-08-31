
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "Engine.h"
#include "Types.h"
#include <iostream>
#include "sokol_time.h"
#include <chrono>
#include <thread>
#include <climits>


using namespace m_Types;

void engine::Engine::e_StartUp(std::shared_ptr<engine::Engine> &e)
{
	graphics->g_StartUp(e->Width, e ->Height, e->window_name, e->window_fullscreen);
	
}

void engine::Engine::e_ShutDown()
{
	
	graphics->g_Shutdown();
	std::cout << "Engine Shutdown"<< std::endl;
}

void engine::Engine::e_ReunGameLoop(std::function<void(std::shared_ptr<engine::Engine> &e)> UpdateCallBack)
{
	int i = 0;
	int input_i = 0;
	std::cout << "Engine is running\n";

	double start_time = glfwGetTime();
	double dt = (1. / 60.);
	double accumulator = 0.0f;
    while (true) {
		
		
		inputs->Update();

		UpdateCallBack;

		graphics->Draw();

		std::cout << "Choose a number the is greater than zero: ";
		std::cin >> input_i;
		if (input_i < 0 || input_i > INT_MAX || i > INT_MAX || i + input_i > INT_MAX) { //Enter any negative number to quit
			break;
		}
		i += input_i;
		std::cout << "You number is: " << i << std::endl;
		

		// Manage timestep
		double end_time = glfwGetTime();
		double frame_time = end_time - start_time;
		start_time = end_time;

		accumulator += frame_time;

		while (accumulator >= dt)
		{
			accumulator -= dt;
		}
		
    }
}
std::function<void(std::shared_ptr<engine::Engine> &e)> engine::Engine::e_UpdateCallBack()
{
	return std::function<void(std::shared_ptr<engine::Engine>&e)>();
}
engine::Engine::Engine() {
	engine::Engine::graphics = std::make_shared<GraphicsManager>();
	engine::Engine::inputs = std::make_shared<InputManager>();
}
