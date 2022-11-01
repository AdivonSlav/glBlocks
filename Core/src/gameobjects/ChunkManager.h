#pragma once

#include <unordered_map>
#include <glm/glm.hpp>
#include <vector>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include "../CoreAPI.h"
#include "Chunk.h"
#include "../graphics/Camera.h"

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
		static std::vector<std::shared_ptr<Chunk>> m_LoadedChunks;
		static std::unique_ptr<std::unordered_map<glm::vec3, std::shared_ptr<Chunk>>> m_PreparedChunks;
		static std::unique_ptr<std::unordered_map<glm::vec3, std::string>> m_MappedChunks;

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
		 * \brief Emplaces the passed chunk to the loaded chunks vector
		 * \param chunk Chunk to be loaded
		 */
		static void LoadChunk(std::shared_ptr<Chunk>& chunk);

		/**
		 * \brief Inserts the chunk with its position as the key to the unordered_map of prepared chunks
		 * \param chunk Chunk to be prepared
		 */
		static void PrepareChunk(std::shared_ptr<Chunk>& chunk);

		/**
		 * \brief Maps the chunk at the given position into the unordered map
		 * \param position Position of the chunk 
		 */
		static void MapChunk(const glm::vec3& position);

		/**
		 * \brief Converts a given three-component vector into a formatted string representing a filename
		 * \param position The position of the chunk
		 * \return The chunk position as a formatted string
		 */
		static const std::string ToFilename(const glm::vec3& position);

		static bool IsLoaded(const glm::vec3& position);
		static bool IsPrepared(const Chunk& chunk);
		static bool IsPrepared(const glm::vec3& position);
		static bool IsMapped(const glm::vec3& position) { return m_MappedChunks->contains(position); }
		static Chunk* GetLoadedChunk(const glm::vec3& position);
		static Chunk* GetPreparedChunk(const glm::vec3& position);

		static std::vector<std::shared_ptr<Chunk>>& GetLoadedChunks() { return m_LoadedChunks; }
		static std::unique_ptr<std::unordered_map<glm::vec3, std::shared_ptr<Chunk>>>& GetPreparedChunks() { return m_PreparedChunks; }
		static std::unordered_map<glm::vec3, std::string>& GetMappedChunks() { return *m_MappedChunks; }
	};
}