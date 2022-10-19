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
		std::unordered_map<glm::vec3, Chunk*>* m_LoadedChunks;
		std::unordered_map<glm::vec3, std::string>* m_UnloadedChunks;
	public:
		ChunkManager();
		~ChunkManager();

		void WriteToFile(glm::vec3 position, const Chunk& chunk);

		Chunk* ReadFromFile(glm::vec3 position);

		void LoadChunks();

		std::unordered_map<glm::vec3, Chunk*>& GetLoadedChunks() { return *m_LoadedChunks; }
		std::unordered_map<glm::vec3, std::string>& GetUnloadedChunks() { return *m_UnloadedChunks; }
	};
}