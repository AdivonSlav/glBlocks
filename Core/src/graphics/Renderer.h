#pragma once

#include "Shader.h"
#include "Texture.h"
#include "../CoreAPI.h"

#include "../gameobjects/ChunkManager.h"

namespace CoreGraphics
{
	enum class ShaderType { BASIC_SHADER, AIR_SHADER };

	class CORE_API Renderer
	{
	private:
		Shader* m_Shader[2] = { nullptr };
	public:
		Renderer();
		~Renderer() = default;

		void Draw();

		/**
		 * \brief Loads the specified shader so it can be used by the renderer
		 * \param shader The shader program to be loaded into the renderer
		 * \param type Whether it is a basic block shader or anything else
		 */
		void LoadShader(Shader& shader, ShaderType type);
	};
}
