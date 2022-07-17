#include <Core.h>

using namespace CoreGraphics;

int main()
{
	auto& instance = Window::Window::Get();

	instance.Init(800, 600, "Test");

	CoreGraphics::Shader testShader("src\\shaders\\vertex.shader", "src\\shaders\\fragment.shader");
	testShader.CreateShaderProgram();

	float positions[] = {
		-0.5f, 0.5f, 1.0f,
		-0.5f,-0.5f, 1.0f,
		 0.5f, 0.5f, 1.0f,
		 0.5f,-0.5f, 1.0f,
	};

	float colors[] = {
		1.0f, 1.0f, 0.2f, 1.0f,
		1.0f, 1.0f, 0.2f, 1.0f,
		1.0f, 1.0f, 0.2f, 1.0f,
		1.0f, 1.0f, 0.2f, 1.0f
	};

	unsigned int indices[] = {
		0, 2, 1,
		2, 1, 3
	};

	VertexArray vao;
	vao.Bind();
	auto posBuffer = new Buffers::VertexBuffer(sizeof(positions), 3, positions, GL_STATIC_DRAW);
	auto colorBuffer = new Buffers::VertexBuffer(sizeof(colors), 4, colors, GL_STATIC_DRAW);
	Buffers::IndexBuffer indexBuffer(sizeof(indices), 3, indices, GL_STATIC_DRAW);

	vao.AddBuffer(posBuffer, 0, GL_FLOAT);
	vao.AddBuffer(colorBuffer, 1, GL_FLOAT);

	Renderer renderer;

	while (!instance.ShouldClose())
	{
		testShader.Bind();
		vao.Bind();

		renderer.Draw();

		instance.PollAndSwapBuffers();
	}
}
