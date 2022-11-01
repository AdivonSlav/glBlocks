#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>

#include "ChunkManager.h"
#include "../utils/Logger.h"

namespace CoreGameObjects
{
	std::vector<std::shared_ptr<Chunk>> ChunkManager::m_LoadedChunks;
	std::unique_ptr<std::unordered_map<glm::vec3, std::shared_ptr<Chunk>>> ChunkManager::m_PreparedChunks = std::make_unique<std::unordered_map<glm::vec3, std::shared_ptr<Chunk>>>();
	std::unique_ptr<std::unordered_map<glm::vec3, std::string>> ChunkManager::m_MappedChunks = std::make_unique<std::unordered_map<glm::vec3, std::string>>();

	void ChunkManager::WriteToFile(glm::vec3 position, Chunk* chunk, unsigned long long& seed)
	{
		// e.g. 32_0_32.ch
		std::string filePath = ToFilename(position);

		std::ofstream stream(filePath, std::ios::trunc | std::ios::binary);

		if (stream.fail())
			LOG_ERROR(strerror(errno));

		if (stream.is_open())
		{
			stream.write(reinterpret_cast<char*>(&seed), sizeof(unsigned long long));
			stream.write(reinterpret_cast<char*>(chunk->GetBlocksPtr()), CHUNK_X * CHUNK_Y * CHUNK_Z * sizeof(signed char));
		}

		stream.close();
	}

	std::shared_ptr<Chunk> ChunkManager::ReadFromFile(glm::vec3 position, unsigned long long& seed)
	{
		auto chunk = std::make_shared<Chunk>(position);
		std::string filePath = ToFilename(position);

		std::ifstream stream(filePath, std::ios::binary);

		if (stream.fail())
			LOG_ERROR(strerror(errno));

		if (stream.is_open())
		{
			stream.read(reinterpret_cast<char*>(&seed), sizeof(unsigned long long));
			stream.read(reinterpret_cast<char*>(chunk->GetBlocksPtr()), CHUNK_X * CHUNK_Y * CHUNK_Z * sizeof(signed char));
		}

		stream.close();

		return chunk;
	}

	bool ChunkManager::IsWritten(glm::vec3 position)
	{
		for (const auto& entry : std::filesystem::directory_iterator(WRITE_PATH))
		{
			std::string path = entry.path().string();

			if (path == ToFilename(position))
				return true;
		}

		return false;
	}

	void ChunkManager::LoadChunk(std::shared_ptr<Chunk>& chunk)
	{
		m_LoadedChunks.emplace_back(chunk);
	}

	void ChunkManager::PrepareChunk(std::shared_ptr<Chunk>& chunk)
	{
		m_PreparedChunks->insert(std::pair(chunk->GetPos(), chunk));
	}

	void ChunkManager::MapChunk(const glm::vec3& position)
	{
		m_MappedChunks->insert(std::pair(position, ToFilename(position)));
	}

	const std::string ChunkManager::ToFilename(const glm::vec3& position)
	{
		return std::format("{}{}_{}_{}.ch", WRITE_PATH, position.x, position.y, position.z);
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

	bool ChunkManager::IsPrepared(const Chunk& chunk)
	{
		return m_PreparedChunks->contains(chunk.GetPos());
	}

	bool ChunkManager::IsPrepared(const glm::vec3& position)
	{
		return m_PreparedChunks->contains(position);
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

	Chunk* ChunkManager::GetPreparedChunk(const glm::vec3& position)
	{
		if (!IsPrepared(position))
		{
			LOG_ERROR("Chunk does not exist!")
			return nullptr;
		}

		return m_PreparedChunks->at(position).get();
	}
}
