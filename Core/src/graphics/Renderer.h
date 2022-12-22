#pragma once

#include "../CoreAPI.h"

#include "Shader.h"
#include "../gameobjects/World.h"
#include "../graphics/Camera.h"

namespace CoreGraphics
{
	enum class ShaderType { BASIC_SHADER, LIGHTSOURCE_SHADER };

	class CORE_API Renderer
	{
	private:
		Shader* m_Shader[2] = { nullptr };
	public:
		Renderer();
		~Renderer() = default;

		/**
		 * \brief Iterates through all of the chunks, applies the necessary uniforms and draws
		 * \param world Reference to the world for lighting information
		 * \param camera Reference to the scene camera
		 */
		void Draw(CoreGameObjects::World& world, Camera& camera);

		/**
		 * \brief Loads the specified shader so it can be used by the renderer
		 * \param shader The shader program to be loaded into the renderer
		 * \param type Whether it is a basic block shader or anything else
		 */
		void LoadShader(Shader& shader, ShaderType type);
	};
}
