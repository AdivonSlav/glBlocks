#include "Chunk.h"
#include "../utils/Logger.h"

#include <glm/glm.hpp>
#include <ctime>

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
		if (x >= CHUNK_X || y >= CHUNK_Y || z >= CHUNK_Z)
		{
			LOG_ERROR("Cannot set block to out of bounds!");
			return;
		}

		m_Blocks[x][y][z] = type;
		m_Rebuild = true;
	}

	BlockType Chunk::GetBlock(int x, int y, int z) const
	{
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

					// Vertex data.
					// Can probably be shortened in some way (too lazy to find out how atm)

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
