#pragma once

#include <unordered_map>
#include <glm/glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include "../CoreAPI.h"
#include "Chunk.h"

namespace CoreGameObjects
{
	class CORE_API ChunkManager
	{
	private:
		std::unordered_map<glm::vec3, Chunk*>* m_Chunks;

	public:
		ChunkManager();
		~ChunkManager();

		std::unordered_map<glm::vec3, Chunk*>& GetLoadedChunks() { return *m_Chunks; }
	};
}