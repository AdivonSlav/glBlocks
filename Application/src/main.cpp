#include <Core.h>

int main()
{
	auto& instance = Window::Window::Get();

	instance.Init(800, 600, "Test");

	while (!instance.ShouldClose())
	{
		instance.PollAndSwapBuffers();
	}
}