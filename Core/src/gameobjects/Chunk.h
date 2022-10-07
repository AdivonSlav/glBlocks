#pragma once

#include "../graphics/VertexArray.h"

#define CHUNK_X 8
#define CHUNK_Y 32
#define CHUNK_Z 8

using namespace CoreGraphics;
using namespace CoreGraphics::Buffers;

namespace CoreGameObjects
{
	enum class BlockType { AIR, GRASS, DIRT, STONE };

	class CORE_API Chunk
	{
	private:
		VertexArray* m_VAO;
		BlockType m_Blocks[CHUNK_X][CHUNK_Y][CHUNK_Z];

		bool m_Rebuild;
		int m_VertexCount;
	public:
		Chunk();
		~Chunk();
		
		void SetBlock(int x, int y, int z, BlockType type);
		BlockType GetBlock(int x, int y, int z) const;

		void Build();

		VertexArray* GetVAO() { return m_VAO; }
		bool GetRebuild() const { return m_Rebuild; }
		int GetVertCount() const { return m_VertexCount; }
	};
}