#include <Core.h>

using namespace CoreGraphics;
using namespace CoreWindow;

// main.cpp is only used for testing purposes. It will be revamped later for the actual application

int main()
{
	auto& instance = Window::Get();

	instance.Init(800, 600, "Test");

	Shader testShader("src\\shaders\\vertex.shader", "src\\shaders\\fragment.shader");
	testShader.CreateShaderProgram();

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

	Renderer renderer;
	Camera camera({0.0f, 0.0f, 2.0f});

	while (!instance.ShouldClose())
	{
		testShader.Bind();
		vao.Bind();
		indexBuffer.Bind();

		camera.SetMatrix(100.0f, 0.1f, 100.0f, testShader, "camMatrix");
		camera.CheckInput();

		renderer.Draw();

		instance.PollAndSwapBuffers();
	}
}
