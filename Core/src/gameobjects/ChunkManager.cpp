#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>

#include "ChunkManager.h"
#include "../utils/Logger.h"

namespace CoreGameObjects
{
	std::unique_ptr<std::unordered_map<glm::vec3, std::shared_ptr<Chunk>>> ChunkManager::m_LoadedChunks = std::make_unique<std::unordered_map<glm::vec3, std::shared_ptr<Chunk>>>();
	std::vector<std::shared_ptr<Chunk>> ChunkManager::m_PreparedChunks;
	std::unique_ptr<std::unordered_map<glm::vec3, std::string>> ChunkManager::m_UnloadedChunks = std::make_unique<std::unordered_map<glm::vec3, std::string>>();

	void ChunkManager::WriteToFile(glm::vec3 position, Chunk* chunk, unsigned long long& seed)
	{
		// e.g. 32_0_32.ch
		std::string chunkName = std::to_string((int)position.x) + "_" + std::to_string((int)position.y) + "_" + std::to_string((int)position.z) + ".ch";
		std::string filePath = WRITE_PATH + chunkName;

		std::ofstream stream(filePath, std::ios::trunc | std::ios::binary);

		if (stream.fail())
		{
			LOG_ERROR(strerror(errno));
		}

		if (stream.is_open())
		{
			stream.write(reinterpret_cast<char*>(&seed), sizeof(unsigned long long));
			stream.write(reinterpret_cast<char*>(chunk->GetBlocksPtr()), CHUNK_X * CHUNK_Y * CHUNK_Z * sizeof(signed char));
		}

		stream.close();

		if (!IsUnloaded(position))
			GetUnloadedChunks().insert(std::pair(position, filePath));
	}

	std::shared_ptr<Chunk> ChunkManager::ReadFromFile(glm::vec3 position, unsigned long long& seed)
	{
		auto chunk = std::make_shared<Chunk>(position);
		std::string filePath = GetUnloadedChunks().at(position);

		std::ifstream stream(filePath, std::ios::binary);

		if (stream.fail())
		{
			LOG_ERROR(strerror(errno));
		}

		if (stream.is_open())
		{
			stream.read(reinterpret_cast<char*>(&seed), sizeof(unsigned long long));
			stream.read(reinterpret_cast<char*>(chunk.get()->GetBlocksPtr()), CHUNK_X * CHUNK_Y * CHUNK_Z * sizeof(signed char));
		}

		stream.close();

		return chunk;
	}

	bool ChunkManager::IsWritten(glm::vec3 position)
	{
		for (const auto& entry : std::filesystem::directory_iterator(WRITE_PATH))
		{
			std::string path = entry.path().string();
			path = path.substr(path.find_last_of('/') + 1);
			path = path.substr(0, path.find('.'));

			size_t strPosition = 0;
			int counter = 0;
			float coordinates[3];

			while ((strPosition = path.find('_')) != std::string::npos)
			{
				coordinates[counter] = std::stof(path.substr(0, strPosition));
				path.erase(0, strPosition + 1);
				counter++;
			}

			coordinates[counter] = std::stof(path);

			if (position == glm::vec3(coordinates[0], coordinates[1], coordinates[2]))
				return true;
		}

		return false;
	}
	void ChunkManager::Cleanup()
	{
		LOG_INFO("Cleaning up chunk arrays...");


	}

	Chunk* ChunkManager::GetLoadedChunk(const glm::vec3& coordinates)
	{
		if (!IsLoaded(coordinates))
			return nullptr;

		return GetLoadedChunks().at(coordinates).get();
	}
}
