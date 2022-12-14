
#include "ChunkManager.h"
#include "TerrainGenerator.h"
#include "../utils/Logger.h"

namespace CoreGameObjects
{
	std::vector<std::shared_ptr<Chunk>> ChunkManager::m_LoadedChunks;
	std::deque<std::future<Chunk*>> ChunkManager::m_QueuedForBuilding;
	std::unordered_set<glm::vec3> ChunkManager::m_QueuedPositionsForBuilding;

	void ChunkManager::Serialize(const glm::vec3& position, Chunk* chunk, unsigned long long& seed)
	{
		// e.g. 32_0_32.ch
		std::string filePath = ToFilename(position);

		std::ofstream stream(filePath, std::ios::trunc | std::ios::binary);

		if (stream.fail())
			LOG_ERROR("Could not serialize chunk to " << filePath);

		if (stream.is_open())
		{
			stream.write(reinterpret_cast<char*>(&seed), sizeof(unsigned long long));
			stream.write(reinterpret_cast<char*>(chunk->GetBlocksPtr()), CHUNK_X * CHUNK_Y * CHUNK_Z * sizeof(signed char));
		}

		chunk->SetSerialized(true);

		stream.close();
	}

	void ChunkManager::Deserialize(const glm::vec3& position, Chunk* chunk, unsigned long long& seed)
	{
		std::string filePath = ToFilename(position);

		std::ifstream stream(filePath, std::ios::binary);

		if (stream.fail())
			LOG_ERROR("Could not deserialize chunk from " << filePath);

		if (stream.is_open())
		{
			stream.read(reinterpret_cast<char*>(&seed), sizeof(unsigned long long));
			stream.read(reinterpret_cast<char*>(chunk->GetBlocksPtr()), CHUNK_X * CHUNK_Y * CHUNK_Z * sizeof(signed char));
		}

		stream.close();
	}

	bool ChunkManager::IsSerialized(const glm::vec3& position)
	{
		return std::filesystem::exists(ToFilename(position));
	}

	void ChunkManager::LoadChunk(const std::shared_ptr<Chunk>& chunk)
	{
		m_LoadedChunks.emplace_back(chunk);
	}

	void ChunkManager::SynchronizeObscured(const Chunk* chunk)
	{
		if (chunk->GetObscuring(0) != nullptr)
			chunk->GetObscuring(0)->SetObscuring(1, nullptr);
		if (chunk->GetObscuring(1) != nullptr)
			chunk->GetObscuring(1)->SetObscuring(0, nullptr);
		if (chunk->GetObscuring(2) != nullptr)
			chunk->GetObscuring(2)->SetObscuring(3, nullptr);
		if (chunk->GetObscuring(3) != nullptr)
			chunk->GetObscuring(3)->SetObscuring(2, nullptr);
	}

	void ChunkManager::QueueForBuild(Chunk* chunk, CoreUtils::Semaphore& semaphore, bool rebuild)
	{
		m_QueuedForBuilding.push_back(std::async(std::launch::async, [](Chunk* chunk, Semaphore& semaphore, bool rebuild)
		{
			std::scoped_lock scopedLock(semaphore);
			std::this_thread::sleep_for(std::chrono::milliseconds(50));

			chunk->Build();

			return chunk;
		}, chunk, std::ref(semaphore), rebuild));

		MarkPositionForBuild(chunk->GetPos());
	}

	bool ChunkManager::IsBuildQueueReady()
	{
		return m_QueuedForBuilding.front().wait_for(std::chrono::seconds(0)) == std::future_status::ready;
	}

	const std::string ChunkManager::ToFilename(const glm::vec3& position)
	{
		return std::format("{}{}_{}.ch", WRITE_PATH, position.x, position.z);
	}

	bool ChunkManager::IsLoaded(const glm::vec3& position)
	{
		for (const auto& loadedChunk : m_LoadedChunks)
		{
			if (loadedChunk->GetPos() == position)
				return true;
		}

		return false;
	}

	Chunk* ChunkManager::GetLoadedChunk(const glm::vec3& position)
	{
		for (const auto& loadedChunk : m_LoadedChunks)
		{
			if (loadedChunk->GetPos() == position)
				return loadedChunk.get();
		}

		return nullptr;
	}
}
