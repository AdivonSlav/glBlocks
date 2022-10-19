#pragma once

#include <glm/vec3.hpp>

#include "../graphics/VertexArray.h"

#define CHUNK_X 16
#define CHUNK_Y 128
#define CHUNK_Z 16

using namespace CoreGraphics;
using namespace CoreGraphics::Buffers;

namespace CoreGameObjects
{
	enum class BlockType { AIR, WATER, GRASS, DIRT, SAND, STONE, IRON_ORE };

	class CORE_API Chunk
	{
	private:
		VertexArray* m_VAO;
		BlockType m_Blocks[CHUNK_X][CHUNK_Y][CHUNK_Z];

		glm::vec3 m_Position;

		bool m_Rebuild;
		int m_VertexCount;
	public:
		Chunk();
		~Chunk();
		
		void SetBlock(int x, int y, int z, BlockType type);
		BlockType GetBlock(int x, int y, int z) const;

		void Build();

		VertexArray* GetVAO() const { return m_VAO; }
		bool GetRebuild() const { return m_Rebuild; }
		int GetVertCount() const { return m_VertexCount; }
		const glm::vec3& GetPos() const { return m_Position; }

		void SetPosition(float x, float y, float z) { m_Position = glm::vec3(x, y, z); }
	};
}