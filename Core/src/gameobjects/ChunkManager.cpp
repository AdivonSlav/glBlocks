#include <string>
#include <fstream>
#include <iostream>

#include "ChunkManager.h"
#include "../utils/Logger.h"

#ifdef BLOCKS_DEBUG
#define WRITE_PATH "src/chunks/"
#else
#define WRITE_PATH "chunks/"
#endif

namespace CoreGameObjects
{
	std::unordered_map<glm::vec3, Chunk*>* ChunkManager::m_LoadedChunks = new std::unordered_map<glm::vec3, Chunk*>();
	std::unordered_map < glm::vec3, std::string>* ChunkManager::m_UnloadedChunks = new std::unordered_map<glm::vec3, std::string>();

	void ChunkManager::WriteToFile(glm::vec3 position, const Chunk& chunk)
	{
		// e.g. 32_0_32.ch
		std::string chunkName = std::to_string((int)position.x) + "_" + std::to_string((int)position.y) + "_" + std::to_string((int)position.z) + ".ch";
		std::string filePath = WRITE_PATH + chunkName;

		std::ofstream stream(filePath, std::fstream::out | std::fstream::trunc);

		if (stream.fail())
		{
			LOG_ERROR(strerror(errno));
		}

		if (stream.is_open())
		{
			for (int x = 0; x < CHUNK_X; x++)
			{
				for (int y = 0; y < CHUNK_Y; y++)
				{
					for (int z = 0; z < CHUNK_Z; z++)
					{
						stream << static_cast<int>(chunk.GetBlock(x, y, z));
					}
				}
			}
		}

		stream.close();

		if (!IsUnloaded(position))
			m_UnloadedChunks->insert(std::pair(position, filePath));
	}

	Chunk* ChunkManager::ReadFromFile(glm::vec3 position)
	{
		auto chunk = new Chunk(position);
		std::string filePath = m_UnloadedChunks->at(position);
		std::ifstream stream(filePath, std::ifstream::in);
		bool airBlock = false;

		if (stream.fail())
		{
			LOG_ERROR(strerror(errno));
			return nullptr;
		}

		if (stream.is_open())
		{
			std::stringstream ss;
			int counter = 0;

			ss << stream.rdbuf();

			std::string fileContents = ss.str();

			for (int x = 0; x < CHUNK_X; x++)
			{
				for (int y = 0; y < CHUNK_Y; y++)
				{
					for (int z = 0; z < CHUNK_Z; z++)
					{
						// Need to subtract 48 to convert from ASCII
						char type = fileContents[counter++];

						if (type == '-')
						{
							type = fileContents[counter++];
							airBlock = true;
						}

						chunk->SetBlock(x, y, z, !airBlock?(BlockType)(type - 48): BlockType::AIR);
						airBlock = false;
					}
				}
			}
		}

		stream.close();

		return chunk;
	}

	void ChunkManager::LoadChunks()
	{
		for (auto& chunk : *m_UnloadedChunks)
		{
			auto generatedChunk = ReadFromFile(chunk.first);
			m_LoadedChunks->insert(std::pair(chunk.first, generatedChunk));
		}
	}

	Chunk* ChunkManager::GetLoadedChunk(const glm::vec3& coordinates)
	{
		if (!IsLoaded(coordinates))
			return nullptr;

		return m_LoadedChunks->at(coordinates);
	}
}
