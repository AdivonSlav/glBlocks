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

	float positions[] = {
		// front
		-1.0f, -1.0f,  1.0,
		 1.0f, -1.0f,  1.0,
		 1.0f,  1.0f,  1.0,
		-1.0f,  1.0f,  1.0,
		// back
		-1.0f, -1.0f, -1.0,
		 1.0f, -1.0f, -1.0,
		 1.0f,  1.0f, -1.0,
		-1.0f,  1.0f, -1.0
	};
	float colors[] = {
		// front colors
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0,
		1.0, 1.0, 1.0,
		// back colors
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0,
		1.0, 1.0, 1.0
	};
	unsigned int indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

	VertexArray vao;
	vao.Bind();
	auto posBuffer = new Buffers::VertexBuffer(sizeof(positions), 3, positions, GL_STATIC_DRAW);
	auto colorBuffer = new Buffers::VertexBuffer(sizeof(colors), 4, colors, GL_STATIC_DRAW);
	Buffers::IndexBuffer indexBuffer(sizeof(indices), 3, indices, GL_STATIC_DRAW);

	vao.AddBuffer(posBuffer, 0, GL_FLOAT);
	vao.AddBuffer(colorBuffer, 1, GL_FLOAT);

	double lastTime = Window::GetTime();

	while (!m_Window.ShouldClose())
	{
		double currentTime = Window::GetTime();
		m_DeltaTime = currentTime - lastTime;
		lastTime = currentTime;
		
		vao.Bind();
		indexBuffer.Bind();
		m_Camera.SetMatrix(65.0f, 0.1f, 1000.0f, basicShader, "camMatrix");
		m_Camera.CheckInput(m_DeltaTime);
		m_Renderer->Draw();

		m_Window.PollAndSwapBuffers();
	}
}

