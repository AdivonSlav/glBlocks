#include "Application.h"
#include "Config.h"

Application::Application()
	: m_Window(Window::Get()), m_Camera({ 0.0f, 0.0f, 2.0f }), m_FrameRate(0.0), m_AverageFrameTime(0.0), m_FrameCount(0), m_LastUpdate(0.0)
{
	m_Window.Init(1366, 768, "glBlocks");
	m_Renderer = new Renderer();
	m_Dashboard = new Dashboard(&m_Camera);
	m_World = new World();
}

Application::~Application()
{
	delete m_Renderer;
	delete m_Dashboard;
	delete m_World;
}

void Application::Run()
{
	Shader basicShader(VERTEX_SHADER, FRAGMENT_SHADER);
	basicShader.CreateShaderProgram();

	Shader lightSourceShader(LIGHT_VERTEX_SHADER, LIGHT_FRAGMENT_SHADER);
	lightSourceShader.CreateShaderProgram();

	m_Renderer->LoadShader(basicShader, ShaderType::BASIC_SHADER);
	m_Renderer->LoadShader(lightSourceShader, ShaderType::LIGHTSOURCE_SHADER);

	std::string texturePath = TEXTURE_PATH;
	texturePath += "texture_atlas.png";
	Texture atlas(GL_TEXTURE_2D, texturePath);
	atlas.Load();
	atlas.Bind(GL_TEXTURE0);
	basicShader.Bind();
	basicShader.SetInt("uAtlasSize", 4);

	m_World->GetGenerator().SetCamera(m_Camera);
	m_World->GetGenerator().Init();

	m_Camera.SetPosition(1, 150.0f, 1);
	m_Camera.SendShader(basicShader);
	m_Camera.SendShader(lightSourceShader);

	auto modelMat = glm::identity<glm::mat4>();

	m_Timer.Start();

	while (!m_Window.ShouldClose())
	{
		m_Camera.SetMatrix(40.0f, 0.1f, 1000.0f);
		m_Camera.CheckInput(m_Timer.GetDelta());

		m_World->StepTime(m_Timer);
		m_World->GetGenerator().LoadChunks();
		m_World->GetGenerator().PrepareChunks();
		m_Renderer->Draw(*m_World, m_Camera);

		m_Dashboard->GetData(m_FrameRate, m_AverageFrameTime);
		m_Dashboard->Render();

		m_Timer.RecordLapse();
		CalcPerf();

		m_Window.PollAndSwapBuffers();
	}

	Dashboard::Cleanup();
	Window::Cleanup();
	LOG_INFO("Ending...");
}

void Application::CalcPerf()
{
	m_FrameCount++;
	m_LastUpdate += m_Timer.GetDelta();

	if (m_LastUpdate >= 1.0)
	{
		m_FrameRate = (double)m_FrameCount * 0.5 + m_FrameRate * 0.5;
		m_FrameCount = 0;
		m_AverageFrameTime = 1000.0 / (m_FrameRate == 0.0 ? 0.001 : m_FrameRate);
		m_LastUpdate = 0.0;
	}
}

