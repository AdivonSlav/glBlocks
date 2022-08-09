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

		m_Shader[(int)ShaderType::BASIC_SHADER]->Bind();
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)0);
	}

	void Renderer::LoadShader(Shader& shader, ShaderType type)
	{
		m_Shader[(int)type] = &shader;
	}
}
