#pragma once

#include <glm/vec3.hpp>

#include "../graphics/VertexArray.h"

#define CHUNK_X 16
#define CHUNK_Y 384
#define CHUNK_Z 16

using namespace CoreGraphics;
using namespace CoreGraphics::Buffers;

namespace CoreGameObjects
{
	enum class BlockType
	{
		UNDEFINED = -2, AIR = -1,

		GRASS = 0,

		DIRT = 2, STONE, SAND
	};

	class CORE_API Chunk
	{
	private:
		VertexArray* m_VAO;
		signed char m_Blocks[CHUNK_X * CHUNK_Y * CHUNK_Z];

		glm::vec3 m_Position;

		bool m_Rebuild;
		bool m_ShouldDispose;
		bool m_ShouldLoad;
		unsigned int m_VertexCount;
	public:
		/**
		 * \brief Constructs the chunk at a default position of (0,0,0)
		 */
		Chunk();

		/**
		 * \brief Constructs the chunk at a specified position
		 * \param position The position to create a chunk at
		 */
		explicit Chunk(const glm::vec3& position);

		~Chunk();

		void SetBlock(int x, int y, int z, BlockType type);
		BlockType GetBlock(int x, int y, int z) const;

		/**
		 * \brief Builds the chunk if it has not been built already or rebuilds it if a change has occurred.
		 */
		void Build();

		VertexArray* GetVAO() const { return m_VAO; }
		signed char* GetBlocksPtr() { return m_Blocks;}
		const glm::vec3& GetPos() const { return m_Position; }
		bool GetRebuild() const { return m_Rebuild; }
		bool ShouldDispose() const { return m_ShouldDispose; }
		bool ShouldLoad() const { return m_ShouldLoad; }
		unsigned int GetVertCount() const { return m_VertexCount; }

		void SetPosition(float x, float y, float z) { m_Position = glm::vec3(x, y, z); }
		void SetPosition(const glm::vec3& position) { m_Position = position; }
		void SetShouldDispose(bool value) { m_ShouldDispose = value; }
		void SetShouldLoad(bool value) { m_ShouldLoad = value; }

		bool operator==(const Chunk& chunk) { return this->GetPos() == chunk.GetPos(); }
	};
}