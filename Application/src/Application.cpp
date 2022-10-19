#include "Application.h"

#ifdef BLOCKS_DEBUG
#define SHADER_PATH "src/shaders/"
#else
#define SHADER_PATH "shaders/"
#endif

Application::Application()
	: m_Window(Window::Get()), m_Camera({0.0f, 0.0f, 2.0f}), m_DeltaTime(0.0), m_ElapsedTime(0.0), m_FrameRate(0.0), m_AverageFrameTime(0.0), m_FrameCount(0)
{
	m_Window.Init(800, 600, "glBlocks");
	m_Renderer = new Renderer();
	m_Dashboard = new Dashboard(true);
	m_Generator = new TerrainGenerator();
}

Application::~Application()
{
	delete m_Renderer;
	delete m_Dashboard;
	delete m_Generator;
}

void Application::Run()
{
	std::string vertexPath = SHADER_PATH;
	vertexPath += "vertex.shader";
	std::string fragmentPath = SHADER_PATH;
	fragmentPath += "fragment.shader";
	Shader basicShader(vertexPath.c_str(), fragmentPath.c_str());
	basicShader.CreateShaderProgram();

	m_Renderer->LoadShader(basicShader, ShaderType::BASIC_SHADER);

	m_Generator->Generate();
	//m_ChunkManager->LoadChunks();
	m_Camera.SetPosition(0, 130.0f, 0);

	auto modelMat = glm::identity<glm::mat4>();

	m_LastTime = steady_clock::now();

	while (!m_Window.ShouldClose())
	{
		m_Camera.SetMatrix(40.0f, 0.1f, 1000.0f, basicShader, "uView", "uProjection");
		basicShader.Bind();
		basicShader.SetMat4<float>("uModel", modelMat);
		m_Camera.CheckInput(m_DeltaTime);

		m_Renderer->Draw();

		CalcTime();

		m_Dashboard->SendData(m_FrameRate, m_AverageFrameTime);
		m_Dashboard->Render();

		m_Window.PollAndSwapBuffers();
	}

	LOG_INFO("Ending...");
}

void Application::CalcTime()
{
	time_point<steady_clock> currentTime = steady_clock::now();

	m_DeltaTime = duration_cast<nanoseconds>(currentTime - m_LastTime).count() / NANO_TO_SECOND;
	m_ElapsedTime += m_DeltaTime;
	m_LastTime = currentTime;
	m_FrameCount++;

	if (m_ElapsedTime >= 1.0)
	{
		m_FrameRate = (double)m_FrameCount * 0.5 + m_FrameRate * 0.5;
		m_FrameCount = 0;
		m_ElapsedTime -= 1.0;
		m_AverageFrameTime = 1000.0 / (m_FrameRate == 0.0 ? 0.001 : m_FrameRate);
	}
}

