#include <glad/glad.h>	
#include "Renderer.h"

namespace CoreGraphics
{
	Renderer::Renderer()
	{

	}

	void Renderer::Draw()
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
}
