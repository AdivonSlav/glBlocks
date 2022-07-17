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
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
	}
}
