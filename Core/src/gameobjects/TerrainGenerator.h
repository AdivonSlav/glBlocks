#pragma once

#include "ChunkManager.h"

namespace CoreGameObjects
{
	class TerrainGenerator
	{
	private:
		ChunkManager& m_ChunkManager;
	public:
		TerrainGenerator(ChunkManager& chunkManager);
		~TerrainGenerator() = default;

		void Generate();
	};
}