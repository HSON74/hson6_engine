
#include "InputManager.h"

void InputManager::StartUp()
{
	//Do something
}

void InputManager::Update() {
	//std::cout << "Input Update" << std::endl;
	glfwPollEvents();
}

bool InputManager::KeyIsPressed(GLFWwindow* window, int key, int action) {
	key_input = glfwGetKey(window, key);
	this->inputKey = key;
	if (key_input == GLFW_PRESS && action == 1) {
		std::cout << "The User press a key" << std::endl;
		return true;
	}
	else if (key_input == GLFW_REPEAT && action == 2) {
		std::cout << "The User press a key repeating" << std::endl;
		return true;
	}
	else if (key_input == GLFW_RELEASE && action == 0) {
		return true;
	}
	else {
		return false;
	}
};


