#include <glad/glad.h>	
#include "Renderer.h"

namespace CoreGraphics
{
	Renderer::Renderer()
	{
		glEnable(GL_DEPTH_TEST);
	}

	void Renderer::Draw()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)0);
	}
}
