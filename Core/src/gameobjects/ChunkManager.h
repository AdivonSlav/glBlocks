#pragma once

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
		static std::queue<std::future<std::shared_ptr<Chunk>>> m_QueuedForBuilding;
		static std::unordered_set<glm::vec3> m_QueuedPositionsForBuilding;

		ChunkManager() = default;
	public:
		/**
		 * \brief Writes the chunk layout information to a file
		 * \param position The position of the chunk
		 * \param chunk A reference to the chunk being written
		 */
		static void Serialize(const glm::vec3& position, Chunk* chunk, unsigned long long& seed);

		/**
		 * \brief Reads the chunk layout information from a file and constructs the chunk
		 * \param position The position of the chunk
		 * \param seed Reference to the TerrainGenerator seed value so it can be updated
		 * \return A newly allocated chunk on the heap from the layout information in the chunk file
		 */
		static std::shared_ptr<Chunk> Deserialize(const glm::vec3& position, unsigned long long& seed);

		/**
		 * \brief Checks whether a chunk file exists corresponding to the given position
		 * \param position Position of the chunk
		 * \return Whether it is written or not
		 */
		static bool IsSerialized(const glm::vec3& position);

		/**
		 * \brief Emplaces the passed chunk to the loaded chunks vector
		 * \param chunk Chunk to be loaded
		 */
		static void LoadChunk(const std::shared_ptr<Chunk>& chunk);

		/**
		 * \brief Checks the obscuring neighbors of the passed chunk and nullpoints this chunk for them. Used just before deallocation to ensure that the deallocated chunk is no longer a neighbor
		 * \param chunk Chunk for which to synchronize
		 */
		static void SynchronizeObscured(const Chunk* chunk);

		static std::shared_ptr<Chunk> GenerateChunk(glm::vec3 position, bool serialized = false);

		static bool IsBuildQueueReady();

		static bool IsPositionQueuedForBuild(const glm::vec3& position) { return m_QueuedPositionsForBuilding.contains(position); }

		static void MarkPositionForBuild(const glm::vec3& position) { m_QueuedPositionsForBuilding.insert(position); }

		static void MarkPositionAsBuilt(const glm::vec3& position) { m_QueuedPositionsForBuilding.erase(position); }

		/**
		 * \brief Converts a given three-component vector into a formatted string representing a filename
		 * \param position The position of the chunk
		 * \return The chunk position as a formatted string
		 */
		static const std::string ToFilename(const glm::vec3& position);

		static bool IsLoaded(const glm::vec3& position);
		static Chunk* GetLoadedChunk(const glm::vec3& position);

		static std::vector<std::shared_ptr<Chunk>>& GetLoadedChunks() { return m_LoadedChunks; }
		static std::queue<std::future<std::shared_ptr<Chunk>>>& GetQueuedForBuild() { return m_QueuedForBuilding; }
	};
}