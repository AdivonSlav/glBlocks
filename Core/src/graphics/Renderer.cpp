#include <glad/glad.h>	
#include "Renderer.h"
#include "../utils/Logger.h"

namespace CoreGraphics
{
	Renderer::Renderer()
	{
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		int textureUnits = 0;
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &textureUnits);
		LOG_INFO("Renderer initialized. Number of texture units: " << textureUnits);
	}

	void Renderer::Draw(const CoreGameObjects::Block& block, const Texture& tex)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.5f, 0.5f, 1.0f);

		if (block.GetType() != CoreGameObjects::BlockType::AIR)
		{
			m_Shader[(int)ShaderType::BASIC_SHADER]->Bind();
			tex.Bind(GL_TEXTURE0);

			m_Shader[(int)ShaderType::BASIC_SHADER]->SetInt("uSampler", 0);
		}
		glDrawElements(GL_TRIANGLES, 6 * 3 * 2, GL_UNSIGNED_SHORT, (void*)0);
	}

	void Renderer::LoadShader(Shader& shader, ShaderType type)
	{
		m_Shader[(int)type] = &shader;
	}
}
