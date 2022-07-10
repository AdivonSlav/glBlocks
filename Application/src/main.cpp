#include <Core.h>
#include <iostream>

int main()
{
	auto& instance = Window::Window::Get();

	instance.Init(800, 600, "Test");

	CoreGraphics::Shader testShader("src\\shaders\\vertex.shader", "src\\shaders\\fragment.shader");
	testShader.CreateShaderProgram();
	testShader.Bind();

	while (!instance.ShouldClose())
	{
		instance.PollAndSwapBuffers();
	}
}
