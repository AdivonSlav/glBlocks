#pragma once

#include <unordered_map>
#include <glm/glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include "../CoreAPI.h"
#include "Chunk.h"
#include "../graphics/Camera.h"
#include "../gameobjects/TerrainGenerator.h"

#ifdef BLOCKS_DEBUG
#define WRITE_PATH "src/chunks/"
#else
#define WRITE_PATH "chunks/"
#endif

namespace CoreGameObjects
{
	class CORE_API ChunkManager
	{
	private:
		static std::unique_ptr<std::unordered_map<glm::vec3, std::shared_ptr<Chunk>>> m_LoadedChunks;
		static std::vector<std::shared_ptr<Chunk>> m_PreparedChunks;
		static std::unique_ptr<std::unordered_map<glm::vec3, std::string>> m_UnloadedChunks;

		ChunkManager() = default;
	public:
		/**
		 * \brief Writes the chunk layout information to a file
		 * \param position The position of the chunk
		 * \param chunk A reference to the chunk being written
		 */
		static void WriteToFile(glm::vec3 position, Chunk* chunk, unsigned long long& seed);

		/**
		 * \brief Reads the chunk layout information from a file and constructs the chunk
		 * \param position The position of the chunk
		 * \return A newly allocated chunk on the heap from the layout information in the chunk file
		 */
		static std::shared_ptr<Chunk> ReadFromFile(glm::vec3 position, unsigned long long& seed);

		/**
		 * \brief Checks whether a chunk file exists corresponding to the given position
		 * \param position Position of the chunk
		 * \return Whether it is written or not
		 */
		static bool IsWritten(glm::vec3 position);

		/**
		 * \brief Deallocates all chunk arrays
		 */
		static void Cleanup();

		static bool IsLoaded(const glm::vec3& position) { return m_LoadedChunks->contains(position); }
		static bool IsUnloaded(const glm::vec3& position) { return m_UnloadedChunks->contains(position); }
		static Chunk* GetLoadedChunk(const glm::vec3& coordinates);

		static std::unordered_map<glm::vec3, std::shared_ptr<Chunk>>& GetLoadedChunks() { return *m_LoadedChunks; }
		static std::vector<std::shared_ptr<Chunk>>& GetPreparedChunks() { return m_PreparedChunks; }
		static std::unordered_map<glm::vec3, std::string>& GetUnloadedChunks() { return *m_UnloadedChunks; }
	};
}