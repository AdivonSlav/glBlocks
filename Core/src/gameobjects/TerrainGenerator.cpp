#include "TerrainGenerator.h"

#include <fstream>
#include <glm/gtc/noise.hpp>

namespace CoreGameObjects
{
	TerrainGenerator::TerrainGenerator(ChunkManager& chunkManager)
		: m_ChunkManager(chunkManager)
	{

	}

	void TerrainGenerator::Generate()
	{
		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				auto chunk = new Chunk();
				glm::vec3 chunkPos(i * CHUNK_X, -CHUNK_Y, j * CHUNK_Z);
				auto pair = std::pair(chunkPos, chunk);

				WriteToFile(pair);

				m_ChunkManager.GetLoadedChunks().insert(pair);
			}
		}
	}

	void TerrainGenerator::WriteToFile(std::pair<glm::vec3, Chunk*> chunk)
	{

	}
}
