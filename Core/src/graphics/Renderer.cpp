#include <glad/glad.h>	

#include "Renderer.h"
#include "../utils/Logger.h"
#include "../gameobjects/ChunkManager.h"

using namespace CoreGameObjects;

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

	void Renderer::Draw(ChunkManager& chunkManager)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.5f, 0.5f, 1.0f);

		for (auto& chunk : chunkManager.GetLoadedChunks())
		{
			auto identity = glm::identity<glm::mat4>();
			auto model = glm::translate(identity, chunk.first);

			if (chunk.second->GetRebuild())
				chunk.second->Build();

			m_Shader[(int)ShaderType::BASIC_SHADER]->Bind();
			m_Shader[(int)ShaderType::BASIC_SHADER]->SetMat4<float>("uModel", model);

			chunk.second->GetVAO()->Bind();
			glDrawArrays(GL_TRIANGLES, 0, chunk.second->GetVertCount());
		}

	}

	void Renderer::LoadShader(Shader& shader, ShaderType type)
	{
		m_Shader[(int)type] = &shader;
	}
}
