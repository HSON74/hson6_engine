#include "GUIManager.h"

void GUIManager::Startup(GLFWwindow* w)
{
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOther(w, true);
	//ImGui_ImplWGPU_Init();

}

void GUIManager::Shutdown()
{
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void GUIManager::Draw()
{
	/*ImGui_ImplWGPU_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::EndFrame(); ImGui::Render();
	ImGui_ImplWGPU_RenderDrawData();*/
}
