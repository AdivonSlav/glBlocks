#include "Game.h"

Game::Game()
	: m_Window(Window::Get()), m_Camera({0.0f, 0.0f, 2.0f}), m_DeltaTime(0.0)
{
	m_Window.Init(800, 600, "glBlocks");
	m_Renderer = new Renderer();
}

Game::~Game()
{
	delete m_Renderer;
}

void Game::Run()
{
	Shader basicShader("src\\shaders\\vertex.shader", "src\\shaders\\fragment.shader");
	basicShader.CreateShaderProgram();

	m_Renderer->LoadShader(basicShader, ShaderType::BASIC_SHADER);

	CoreGameObjects::Block testBlock(CoreGameObjects::BlockType::DIRT, { 1.0f, 1.0f, 1.0f }, 1.0f);

	double lastTime = Window::GetTime();

	while (!m_Window.ShouldClose())
	{
		double currentTime = Window::GetTime();
		m_DeltaTime = currentTime - lastTime;
		lastTime = currentTime;

		testBlock.GetVAO()->Bind();
		testBlock.GetIBO()->Bind();

		basicShader.SetFloat("uTransparency", 0.3f);

		m_Camera.SetMatrix(40.0f, 0.1f, 1000.0f, basicShader, "uView", "uProjection");
		m_Camera.CheckInput(m_DeltaTime);
		m_Renderer->Draw();

		m_Window.PollAndSwapBuffers();
	}
}

