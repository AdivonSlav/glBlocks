#include "Dashboard.h"
#include "../graphics/Shader.h"
#include "../window/Window.h"

#include "../vendor/imgui/imgui.h"
#include "../vendor/imgui/imgui_impl_glfw.h"
#include "../vendor/imgui/imgui_impl_opengl3.h"

namespace CoreUtils
{
	Dashboard::Dashboard(bool show, CoreGraphics::Camera* camera)
		: m_Show(show), m_Camera(camera)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		ImGui::StyleColorsDark();

		ImGui_ImplGlfw_InitForOpenGL(CoreWindow::Window::GetWindowPtr(), true);
		ImGui_ImplOpenGL3_Init(BLOCKS_GLSL_VERSION);
	}

	Dashboard::~Dashboard()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void Dashboard::Render()
	{
		if (m_Show)
		{
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			PrepareWindows();

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}
	}

	void Dashboard::PrepareWindows()
	{
		ImGui::Begin("Performance");

		ImGui::Text("FPS: %.2f", m_FPS);
		ImGui::SameLine();
		ImGui::Text("Average frametime: %.3f ms", m_AverageFrameTime);
		ImGui::Text("Camera position (XYZ): %.2f %.2f %.2f", m_Camera->GetPosition().x, m_Camera->GetPosition().y, m_Camera->GetPosition().z);

		ImGui::End();
	}

	void Dashboard::SendData(double fps, double avgFrameTime)
	{
		m_FPS = fps;
		m_AverageFrameTime = avgFrameTime;
	}
}
