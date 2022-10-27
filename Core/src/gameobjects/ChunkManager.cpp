#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>

#include "ChunkManager.h"
#include "../utils/Logger.h"

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

	void ChunkManager::MapChunks()
	{
		int chCounter = 0;

		for (const auto& entry : std::filesystem::directory_iterator(WRITE_PATH))
		{
			std::string path = entry.path().string();
			path = path.substr(path.find_last_of('/') + 1);
			path = path.substr(0, path.find('.'));

			size_t position = 0;
			int counter = 0;
			float coordinates[3];

			while ((position = path.find('_')) != std::string::npos)
			{
				coordinates[counter] = std::stof(path.substr(0, position));
				path.erase(0, position + 1);
				counter++;
			}

			coordinates[counter] = std::stof(path);
			std::string path_temp = entry.path().string();

			m_UnloadedChunks->insert(std::pair(glm::vec3(coordinates[0], coordinates[1], coordinates[2]), path_temp.c_str()));

			chCounter++;
		}

		LOG_INFO("Mapped " << chCounter << " chunks from disk");
	}

	void ChunkManager::LoadChunks()
	{
		for (auto& chunk : *m_UnloadedChunks)
		{
			auto generatedChunk = ReadFromFile(chunk.first);
			m_LoadedChunks->insert(std::pair(chunk.first, generatedChunk));
		}
	}

	void ChunkManager::Cleanup()
	{
		LOG_INFO("Cleaning up chunk arrays...");

		for (auto& chunk : *m_LoadedChunks)
			delete chunk.second;

		delete m_LoadedChunks;
		delete m_UnloadedChunks;
	}

	Chunk* ChunkManager::GetLoadedChunk(const glm::vec3& coordinates)
	{
		if (!IsLoaded(coordinates))
			return nullptr;

		return m_LoadedChunks->at(coordinates);
	}
}
