#include "Application.h"

Application::Application()
	: m_Window(Window::Get()), m_Camera({0.0f, 0.0f, 2.0f}), m_DeltaTime(0.0), m_ElapsedTime(0.0), m_FrameRate(0.0), m_AverageFrameTime(0.0), m_FrameCount(0)
{
	m_Window.Init(800, 600, "glBlocks");
	m_Renderer = new Renderer();
	m_ChunkManager = new ChunkManager();
	m_Dashboard = new Dashboard(true);
}

Application::~Application()
{
	delete m_Renderer;
	delete m_ChunkManager;
	delete m_Dashboard;
}

void Application::Run()
{
	Shader basicShader("src\\shaders\\vertex.shader", "src\\shaders\\fragment.shader");
	basicShader.CreateShaderProgram();
	//Shader airShader("src\\shaders\\air_vertex.shader", "src\\shaders\\air_fragment.shader");
	//airShader.CreateShaderProgram();

	m_Renderer->LoadShader(basicShader, ShaderType::BASIC_SHADER);
	//m_Renderer->LoadShader(airShader, ShaderType::AIR_SHADER);

	//Texture dirtBlock(GL_TEXTURE_2D, "src\\textures\\grass_block_bottom.png");
	//dirtBlock.Load();

	auto modelMat = glm::identity<glm::mat4>();

	m_LastTime = steady_clock::now();

	while (!m_Window.ShouldClose())
	{
		m_Camera.SetMatrix(40.0f, 0.1f, 1000.0f, basicShader, "uView", "uProjection");
		basicShader.Bind();
		basicShader.SetMat4<float>("uModel", modelMat);
		m_Camera.CheckInput(m_DeltaTime);

		m_Renderer->Draw(*m_ChunkManager);

		CalcTime();

		m_Dashboard->SendData(m_FrameRate, m_AverageFrameTime);
		m_Dashboard->Render();


		m_Window.PollAndSwapBuffers();
	}
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

