#pragma once

#include "../CoreAPI.h"
#include "../utils/Semaphore.h"
#include "Chunk.h"

#define WRITE_PATH "chunks/"

namespace CoreGameObjects
{
	using namespace CoreUtils;

	class CORE_API ChunkManager
	{
	private:
		static std::vector<std::shared_ptr<Chunk>> m_LoadedChunks;
		static std::deque<std::future<std::vector<Chunk*>>> m_QueuedForBuilding;
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
		 */
		static void Deserialize(const glm::vec3& position, Chunk* chunk, unsigned long long& seed);

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

		/**
		 * \brief Queues a vector of chunks as std::future objects that will be built in a separate thread
		 * \param chunks Vector of chunks that need to be built
		 * \param semaphore A semaphore that prevents the spawning of too many concurrent threads
		 * \param rebuild Whether the chunks need to be rebuilt or not
		 */
		static void QueueForBuild(std::vector<Chunk*>& chunks, CoreUtils::Semaphore& semaphore, bool rebuild = false);

		/**
		 * \brief Returns whether the front of the future deque has finished its task
		 * \return True/false whether the future is ready
		 */
		static bool IsBuildQueueReady();

		/**
		 * \brief Checks if the set of queued build positions contains the provided position
		 * \param position Position of the chunk
		 * \return Whether the provided chunk position is already being built
		 */
		static bool IsPositionQueuedForBuild(const glm::vec3& position) { return m_QueuedPositionsForBuilding.count(position) > 0; }

		/**
		 * \brief Inserts the provided position, thereby marking the chunk at that position as already being operated on by another thread (being built)
		 * \param position Position of the chunk
		 */
		static void MarkPositionForBuild(const glm::vec3& position) { m_QueuedPositionsForBuilding.insert(position); }

		/**
		 * \brief Marks the chunk at that position as having been built and no longer being operated on
		 * \param position Position of the chunk
		 */
		static void MarkPositionAsBuilt(const glm::vec3& position) { m_QueuedPositionsForBuilding.erase(position); }

		/**
		 * \brief Converts a given three-component vector into a formatted string representing a filename
		 * \param position The position of the chunk
		 * \return The chunk position as a formatted string
		 */
		static const std::string ToFilename(const glm::vec3& position);

		/**
		 * \brief Checks whether the chunk at the provided position is loaded into memory
		 * \param position Position of the chunk
		 * \return Whether the chunk exists or not
		 */
		static bool IsLoaded(const glm::vec3& position);

		/**
		 * \brief Returns the chunk at the provied position as a pointer
		 * \param position Position of the chunk
		 * \return Pointer to the chunk in memory
		 */
		static Chunk* GetLoadedChunk(const glm::vec3& position);

		static std::vector<std::shared_ptr<Chunk>>& GetLoadedChunks() { return m_LoadedChunks; }
		static std::deque<std::future<std::vector<Chunk*>>>& GetQueuedForBuild() { return m_QueuedForBuilding; }
	};
}