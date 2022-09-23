#include <glad/glad.h>	
#include "Renderer.h"

namespace CoreGraphics
{
	Renderer::Renderer()
	{
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void Renderer::Draw()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.5f, 0.5f, 1.0f);

		m_Shader[(int)ShaderType::BASIC_SHADER]->Bind();
		glDrawElements(GL_TRIANGLES, 6 * 3 * 2, GL_UNSIGNED_SHORT, (void*)0);
	}

	void Renderer::LoadShader(Shader& shader, ShaderType type)
	{
		m_Shader[(int)type] = &shader;
	}
}
