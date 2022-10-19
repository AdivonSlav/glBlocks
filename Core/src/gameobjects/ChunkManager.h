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
		static std::unordered_map<glm::vec3, Chunk*>* m_LoadedChunks;
		static std::unordered_map<glm::vec3, std::string>* m_UnloadedChunks;

		ChunkManager() = default;
	public:
		static void WriteToFile(glm::vec3 position, const Chunk& chunk);
		static Chunk* ReadFromFile(glm::vec3 position);

		static void LoadChunks();

		static bool IsLoaded(const glm::vec3& coordinates) { return m_LoadedChunks->contains(coordinates); }
		static Chunk* GetLoadedChunk(const glm::vec3& coordinates);

		static std::unordered_map<glm::vec3, Chunk*>& GetLoadedChunks() { return *m_LoadedChunks; }
		static std::unordered_map<glm::vec3, std::string>& GetUnloadedChunks() { return *m_UnloadedChunks; }
	};
}