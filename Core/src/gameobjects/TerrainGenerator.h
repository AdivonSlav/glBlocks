#pragma once

#include "ChunkManager.h"

namespace CoreGameObjects
{
	class CORE_API TerrainGenerator
	{
	private:
		ChunkManager& m_ChunkManager;
	public:
		TerrainGenerator(ChunkManager& chunkManager);
		~TerrainGenerator() = default;

		void Generate();

		void WriteToFile(std::pair<glm::vec3, Chunk*> chunk);
	};
}