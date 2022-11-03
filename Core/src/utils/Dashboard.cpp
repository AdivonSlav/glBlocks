
#include "Dashboard.h"
#include "../graphics/Shader.h"
#include "../window/Window.h"

using namespace CoreWindow;

namespace CoreUtils
{
	bool Dashboard::m_Show = false;
	Payload Dashboard::m_Payload;

	Dashboard::Dashboard(CoreGraphics::Camera* camera)
		: m_Camera(camera)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		ImGui::StyleColorsDark();

		ImGui_ImplGlfw_InitForOpenGL(CoreWindow::Window::GetWindowPtr(), true);
		ImGui_ImplOpenGL3_Init(BLOCKS_GLSL_VERSION);
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
		ImGui::SliderFloat("Time step multiplier:", &m_Payload.timeStepMultiplier, 0.0f, 20.0f);

		ImGui::End();
	}

	void Dashboard::GetData(double fps, double avgFrameTime)
	{
		m_FPS = fps;
		m_AverageFrameTime = avgFrameTime;
	}

	void Dashboard::Cleanup()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
}
