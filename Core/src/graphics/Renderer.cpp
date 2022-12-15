#include "PCH.h"



#include "Renderer.h"
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

	void Renderer::Draw(World& world, Camera& camera)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(world.GetClearColor().r, world.GetClearColor().g, world.GetClearColor().b, 1.0f);

		m_Shader[(int)ShaderType::BASIC_SHADER]->Bind();

		for (auto& chunk : ChunkManager::GetLoadedChunks())
		{
			if (!chunk->IsUploaded() || !chunk->ShouldRender())
				continue;

			if (chunk->GetVAO() == nullptr)
				__debugbreak();

			auto identity = glm::identity<glm::mat4>();
			auto model = glm::translate(identity, chunk->GetPos());

			m_Shader[(int)ShaderType::BASIC_SHADER]->SetMat4<float>("uModel", model);
			m_Shader[(int)ShaderType::BASIC_SHADER]->SetVec3<float>("uLightPos", world.GetLightPosition());
			m_Shader[(int)ShaderType::BASIC_SHADER]->SetVec3<float>("uCamPos", camera.GetPosition());
			m_Shader[(int)ShaderType::BASIC_SHADER]->SetFloat("uLightLevel", world.GetLightLevel());
			m_Shader[(int)ShaderType::BASIC_SHADER]->SetInt("uSunHasSet", world.GetHasSet());

			chunk->GetVAO()->Bind();
			glDrawArrays(GL_TRIANGLES, 0, chunk->GetVertCount());
			chunk->GetVAO()->Unbind();
		}

		m_Shader[(int)ShaderType::BASIC_SHADER]->Unbind();

		m_Shader[(int)ShaderType::LIGHTSOURCE_SHADER]->Bind();

		m_Shader[(int)ShaderType::LIGHTSOURCE_SHADER]->SetVec3<float>("uCamRight", camera.GetWorldRight());
		m_Shader[(int)ShaderType::LIGHTSOURCE_SHADER]->SetVec3<float>("uCamUp", camera.GetWorldUp());

		for (int i = 0; i < 2; i++)
		{
			m_Shader[(int)ShaderType::LIGHTSOURCE_SHADER]->SetVec3<float>("uWorldPos",world.GetLightSource((LightSourceType)i).GetPosition());
			m_Shader[(int)ShaderType::LIGHTSOURCE_SHADER]->SetFloat("uSize", world.GetLightSource((LightSourceType)i).GetSize());
			world.GetLightSource((LightSourceType)i).GetVAO().Bind();
			world.GetLightSource((LightSourceType)i).GetIBO()->Bind();
			glDrawElements(GL_TRIANGLES, world.GetLightSource((LightSourceType)i).GetIndexCount(), GL_UNSIGNED_SHORT, (const void*)0);
			world.GetLightSource((LightSourceType)i).GetIBO()->Unbind();
			world.GetLightSource((LightSourceType)i).GetVAO().Unbind();
		}
		m_Shader[(int)ShaderType::LIGHTSOURCE_SHADER]->Unbind();
	}

	void Renderer::LoadShader(Shader& shader, ShaderType type)
	{
		m_Shader[(int)type] = &shader;
	}
}
