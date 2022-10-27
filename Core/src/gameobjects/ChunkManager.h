#pragma once

#include <unordered_map>
#include <glm/glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include "../CoreAPI.h"
#include "Chunk.h"

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
		static std::unordered_map<glm::vec3, Chunk*>* m_LoadedChunks;
		static std::unordered_map<glm::vec3, std::string>* m_UnloadedChunks;

		ChunkManager() = default;
	public:
		/**
		 * \brief Writes the chunk layout information to a file
		 * \param position The position of the chunk
		 * \param chunk A reference to the chunk being written
		 */
		static void WriteToFile(glm::vec3 position, const Chunk& chunk);

		/**
		 * \brief Reads the chunk layout information from a file and constructs the chunk
		 * \param position The position of the chunk
		 * \return A newly allocated chunk on the heap from the layout information in the chunk file
		 */
		static Chunk* ReadFromFile(glm::vec3 position);

		static void MapChunks();

		/**
		 * \brief Loads any unloaded chunks
		 */
		static void LoadChunks();

		/**
		 * \brief Deallocates all chunk arrays
		 */
		static void Cleanup();

		static bool IsLoaded(const glm::vec3& position) { return m_LoadedChunks->contains(position); }
		static bool IsUnloaded(const glm::vec3& position) { return m_UnloadedChunks->contains(position); }
		static Chunk* GetLoadedChunk(const glm::vec3& coordinates);

		static std::unordered_map<glm::vec3, Chunk*>& GetLoadedChunks() { return *m_LoadedChunks; }
		static std::unordered_map<glm::vec3, std::string>& GetUnloadedChunks() { return *m_UnloadedChunks; }
	};
}