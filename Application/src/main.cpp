#include <Core.h>

int main()
{
	auto& instance = Window::Window::Get();

	instance.Init(800, 600, "Test");

	CoreGraphics::Shader testShader("src\\shaders\\vertex.shader", "src\\shaders\\fragment.shader");
	testShader.CreateShaderProgram();

	float posBuffer[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	float colorBuffer[] = {
		1.0f, 1.0f, 0.2f, 1.0f,
		1.0f, 1.0f, 0.2f, 1.0f,
		1.0f, 1.0f, 0.2f, 1.0f
	};

	CoreGraphics::VertexArray vao;
	auto posVBO = new CoreGraphics::Buffer(GL_ARRAY_BUFFER, sizeof(posBuffer), 3, posBuffer, GL_STATIC_DRAW);
	auto colorVBO = new CoreGraphics::Buffer(GL_ARRAY_BUFFER, sizeof(colorBuffer), 4, colorBuffer, GL_STATIC_DRAW);

	vao.AddBuffer(posVBO, 0, GL_FLOAT);
	vao.AddBuffer(colorVBO, 1, GL_FLOAT);

	CoreGraphics::Renderer renderer;

	while (!instance.ShouldClose())
	{
		testShader.Bind();
		vao.Bind();

		renderer.Draw();

		instance.PollAndSwapBuffers();
	}
}
