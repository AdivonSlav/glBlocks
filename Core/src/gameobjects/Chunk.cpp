#include "Chunk.h"
#include "../utils/Logger.h"
#include "ChunkManager.h"

#include <glm/glm.hpp>

namespace CoreGameObjects
{
	Chunk::Chunk()
		: m_VAO(new VertexArray()), m_Position(0.0f, 0.0f, 0.0f), m_Rebuild(false), m_VertexCount(0)
	{

	}

	Chunk::~Chunk()
	{
		delete m_VAO;
	}

	void Chunk::SetBlock(int x, int y, int z, BlockType type)
	{
		if (x >= CHUNK_X || y >= CHUNK_Y || z >= CHUNK_Z
			|| x < 0 || y < 0 || z < 0)
		{
			LOG_ERROR("Cannot set block to out of bounds!");
			return;
		}

		m_Blocks[x][y][z] = type;
		m_Rebuild = true;
	}

	BlockType Chunk::GetBlock(int x, int y, int z) const
	{
		if (x >= CHUNK_X || y >= CHUNK_Y || z >= CHUNK_Z
			|| x < 0 || y < 0 || z < 0)
		{
			return BlockType::UNDEFINED;
		}

		return m_Blocks[x][y][z];
	}

	void Chunk::Build()
	{
		m_Rebuild = false;

		// The position layout contains one extra byte per vertex in order to have 4-byte alignment which is unused.
		// Could probably change around the data types in order to maximize usage

		auto positions = new glm::tvec4<GLbyte>[CHUNK_X * CHUNK_Y * CHUNK_Z * 6 * 6];
		auto types = new GLbyte[CHUNK_X * CHUNK_Y * CHUNK_Z * 6 * 6];

		int counter = 0;

		for (int x = 0; x < CHUNK_X; x++)
		{
			for (int y = 0; y < CHUNK_Y; y++)
			{
				for (int z = 0; z < CHUNK_Z; z++)
				{
					auto type = m_Blocks[x][y][z];

					if (type == BlockType::AIR)
						continue;

					auto frontType = GetBlock(x, y, z + 1);
					auto backType = GetBlock(x, y, z - 1);
					auto leftType = GetBlock(x - 1, y, z);
					auto rightType = GetBlock(x + 1, y, z);
					auto topType = GetBlock(x, y + 1, z);
					auto bottomType = GetBlock(x, y - 1, z);

					// Gets all the chunks whose blocks might be obscuring this one, if nullptr then no chunk borders the current one
					auto obscuringChunkFront = ChunkManager::GetLoadedChunk({ m_Position.x, 0.0f, m_Position.z + CHUNK_Z });
					auto obscuringChunkBack = ChunkManager::GetLoadedChunk({ m_Position.x, 0.0f, m_Position.z - CHUNK_Z });
					auto obscuringChunkLeft = ChunkManager::GetLoadedChunk({ m_Position.x - CHUNK_X, 0.0f, m_Position.z });
					auto obscuringChunkRight = ChunkManager::GetLoadedChunk({ m_Position.x + CHUNK_X, 0.0f, m_Position.z });

					// Incase a bordering chunk exists, check whether its neighboring block is obscuring the current one
					if (obscuringChunkFront)
						obscuringChunkFront = obscuringChunkFront->GetBlock(x, y, 0) == BlockType::AIR ? nullptr : obscuringChunkFront;
					if (obscuringChunkBack)
						obscuringChunkBack = obscuringChunkBack->GetBlock(x, y, CHUNK_Z - 1) == BlockType::AIR ? nullptr : obscuringChunkBack;
					if (obscuringChunkLeft)
						obscuringChunkLeft = obscuringChunkLeft->GetBlock(CHUNK_X - 1, y, z) == BlockType::AIR ? nullptr : obscuringChunkLeft;
					if (obscuringChunkRight)
						obscuringChunkRight = obscuringChunkRight->GetBlock(0, y, z) == BlockType::AIR ? nullptr : obscuringChunkRight;

					// Vertex data).
					// Can probably be shortened in some way (too lazy to find out how atm)
					// Each voxel face is only created if there is no adjacent face obscuring it

					// Front
					if (frontType == BlockType::UNDEFINED && !obscuringChunkFront || frontType == BlockType::AIR)
					{
						positions[counter++] = { x + 1, y + 1, z + 1, 0 };
						types[counter - 1] = (GLbyte)(type);
						positions[counter++] = { x, y + 1, z + 1, 0 };
						types[counter - 1] = (GLbyte)(type);
						positions[counter++] = { x, y, z + 1, 0 };
						types[counter - 1] = (GLbyte)(type);
						positions[counter++] = { x, y, z + 1, 0 };
						types[counter - 1] = (GLbyte)(type);
						positions[counter++] = { x + 1, y, z + 1, 0 };
						types[counter - 1] = (GLbyte)(type);
						positions[counter++] = { x + 1, y + 1, z + 1, 0 };
						types[counter - 1] = (GLbyte)(type);
					}

					// Back
					if (backType == BlockType::UNDEFINED && !obscuringChunkBack || backType == BlockType::AIR)
					{
						positions[counter++] = { x, y + 1, z, 0 };
						types[counter - 1] = (GLbyte)(type);
						positions[counter++] = { x + 1, y + 1, z, 0 };
						types[counter - 1] = (GLbyte)(type);
						positions[counter++] = { x + 1, y, z, 0 };
						types[counter - 1] = (GLbyte)(type);
						positions[counter++] = { x + 1, y, z, 0 };
						types[counter - 1] = (GLbyte)(type);
						positions[counter++] = { x, y, z, 0 };
						types[counter - 1] = (GLbyte)(type);
						positions[counter++] = { x, y + 1, z, 0 };
						types[counter - 1] = (GLbyte)(type);
					}

					// Left
					if (leftType == BlockType::UNDEFINED && !obscuringChunkLeft || leftType == BlockType::AIR)
					{
						positions[counter++] = { x, y + 1, z + 1, 0 };
						types[counter - 1] = (GLbyte)(type);
						positions[counter++] = { x, y + 1, z, 0 };
						types[counter - 1] = (GLbyte)(type);
						positions[counter++] = { x, y, z, 0 };
						types[counter - 1] = (GLbyte)(type);
						positions[counter++] = { x, y, z, 0 };
						types[counter - 1] = (GLbyte)(type);
						positions[counter++] = { x, y, z + 1, 0 };
						types[counter - 1] = (GLbyte)(type);
						positions[counter++] = { x, y + 1, z + 1, 0 };
						types[counter - 1] = (GLbyte)(type);
					}

					// Right
					if (rightType == BlockType::UNDEFINED && !obscuringChunkRight || rightType == BlockType::AIR)
					{
						positions[counter++] = { x + 1, y + 1, z, 0 };
						types[counter - 1] = (GLbyte)(type);
						positions[counter++] = { x + 1, y + 1, z + 1, 0 };
						types[counter - 1] = (GLbyte)(type);
						positions[counter++] = { x + 1, y, z + 1, 0 };
						types[counter - 1] = (GLbyte)(type);
						positions[counter++] = { x + 1, y, z + 1, 0 };
						types[counter - 1] = (GLbyte)(type);
						positions[counter++] = { x + 1, y, z, 0 };
						types[counter - 1] = (GLbyte)(type);
						positions[counter++] = { x + 1, y + 1, z, 0 };
						types[counter - 1] = (GLbyte)(type);
					}

					// Top
					if (topType== BlockType::UNDEFINED || topType == BlockType::AIR)
					{
						positions[counter++] = { x + 1, y + 1, z, 0 };
						types[counter - 1] = (GLbyte)(type);
						positions[counter++] = { x , y + 1, z, 0 };
						types[counter - 1] = (GLbyte)(type);
						positions[counter++] = { x, y + 1, z + 1, 0 };
						types[counter - 1] = (GLbyte)(type);
						positions[counter++] = { x, y + 1, z + 1, 0 };
						types[counter - 1] = (GLbyte)(type);
						positions[counter++] = { x + 1, y + 1, z + 1, 0 };
						types[counter - 1] = (GLbyte)(type);
						positions[counter++] = { x + 1, y + 1, z, 0 };
						types[counter - 1] = (GLbyte)(type);
					}

					// Bottom
					if (bottomType == BlockType::UNDEFINED || bottomType == BlockType::AIR)
					{
						positions[counter++] = { x, y, z, 0 };
						types[counter - 1] = (GLbyte)(type);
						positions[counter++] = { x + 1, y, z, 0 };
						types[counter - 1] = (GLbyte)(type);
						positions[counter++] = { x + 1, y, z + 1, 0 };
						types[counter - 1] = (GLbyte)(type);
						positions[counter++] = { x + 1, y, z + 1, 0 };
						types[counter - 1] = (GLbyte)(type);
						positions[counter++] = { x, y, z + 1, 0 };
						types[counter - 1] = (GLbyte)(type);
						positions[counter++] = { x, y, z, 0 };
						types[counter - 1] = (GLbyte)(type);
					}
				}
			}
		}

		unsigned int posBufferSize = CHUNK_X * CHUNK_Y * CHUNK_Z * 6 * 6 * 4 * sizeof(GLbyte);
		unsigned int typeBufferSize = CHUNK_X * CHUNK_Y * CHUNK_Z * 6 * 6 * sizeof(GLbyte);

		m_VAO->Bind();
		auto posBuffer = new VertexBuffer(posBufferSize, 4, positions, GL_STATIC_DRAW);
		m_VAO->AddBuffer(posBuffer, 0, GL_BYTE);

		auto typeBuffer = new VertexBuffer(typeBufferSize, 1, types, GL_STATIC_DRAW);
		m_VAO->AddBuffer(typeBuffer, 1, GL_BYTE);
		m_VAO->Unbind();

		m_VertexCount = counter;

		delete[] positions;
		delete[] types;
	}
}
