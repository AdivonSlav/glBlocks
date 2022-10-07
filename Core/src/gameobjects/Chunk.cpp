#include "Chunk.h"
#include <glm/glm.hpp>
#include <ctime>

namespace CoreGameObjects
{
	Chunk::Chunk()
		: m_VAO(new VertexArray()), m_Rebuild(false), m_VertexCount(0)
	{
		srand(unsigned(time(NULL)));

		for (int x = 0; x < CHUNK_X; x++)
		{
			for (int y = 0; y < CHUNK_Y; y++)
			{
				for (int z = 0; z < CHUNK_Z; z++)
				{
					auto type = static_cast<BlockType>(rand() % 4);
					SetBlock(x, y, z, type);
				}
			}
		}

	}

	Chunk::~Chunk()
	{
		delete m_VAO;
	}

	void Chunk::SetBlock(int x, int y, int z, BlockType type)
	{
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

		glm::tvec3<GLbyte> positions[CHUNK_X * CHUNK_Y * CHUNK_Z * 6 * 6];
		GLbyte types[CHUNK_X * CHUNK_Y * CHUNK_Z * 6 * 6];

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

					positions[counter++] = { x + 1, y + 1, z + 1 };
					types[counter - 1] = (GLbyte)(type);
					positions[counter++] = { x, y + 1, z + 1 };
					types[counter - 1] = (GLbyte)(type);
					positions[counter++] = { x, y, z + 1 };
					types[counter - 1] = (GLbyte)(type);
					positions[counter++] = { x, y, z + 1 };
					types[counter - 1] = (GLbyte)(type);
					positions[counter++] = { x + 1, y, z + 1 };
					types[counter - 1] = (GLbyte)(type);
					positions[counter++] = { x + 1, y + 1, z + 1 };
					types[counter - 1] = (GLbyte)(type);

					positions[counter++] = { x, y + 1, z };
					types[counter - 1] = (GLbyte)(type);
					positions[counter++] = { x + 1, y + 1, z };
					types[counter - 1] = (GLbyte)(type);
					positions[counter++] = { x + 1, y, z };
					types[counter - 1] = (GLbyte)(type);
					positions[counter++] = { x + 1, y, z };
					types[counter - 1] = (GLbyte)(type);
					positions[counter++] = { x, y, z };
					types[counter - 1] = (GLbyte)(type);
					positions[counter++] = { x, y + 1, z };
					types[counter - 1] = (GLbyte)(type);

					positions[counter++] = { x, y + 1, z + 1 };
					types[counter - 1] = (GLbyte)(type);
					positions[counter++] = { x, y + 1, z };
					types[counter - 1] = (GLbyte)(type);
					positions[counter++] = { x, y, z };
					types[counter - 1] = (GLbyte)(type);
					positions[counter++] = { x, y, z };
					types[counter - 1] = (GLbyte)(type);
					positions[counter++] = { x, y, z + 1 };
					types[counter - 1] = (GLbyte)(type);
					positions[counter++] = { x, y + 1, z + 1 };
					types[counter - 1] = (GLbyte)(type);

					positions[counter++] = { x + 1, y + 1, z };
					types[counter - 1] = (GLbyte)(type);
					positions[counter++] = { x + 1, y + 1, z + 1 };
					types[counter - 1] = (GLbyte)(type);
					positions[counter++] = { x + 1, y, z + 1 };
					types[counter - 1] = (GLbyte)(type);
					positions[counter++] = { x + 1, y, z + 1 };
					types[counter - 1] = (GLbyte)(type);
					positions[counter++] = { x + 1, y, z };
					types[counter - 1] = (GLbyte)(type);
					positions[counter++] = { x + 1, y + 1, z };
					types[counter - 1] = (GLbyte)(type);

					positions[counter++] = { x + 1, y + 1, z };
					types[counter - 1] = (GLbyte)(type);
					positions[counter++] = { x , y + 1, z };
					types[counter - 1] = (GLbyte)(type);
					positions[counter++] = { x, y + 1, z + 1 };
					types[counter - 1] = (GLbyte)(type);
					positions[counter++] = { x, y + 1, z + 1 };
					types[counter - 1] = (GLbyte)(type);
					positions[counter++] = { x + 1, y + 1, z + 1 };
					types[counter - 1] = (GLbyte)(type);
					positions[counter++] = { x + 1, y + 1, z };
					types[counter - 1] = (GLbyte)(type);

					positions[counter++] = { x, y, z };
					types[counter - 1] = (GLbyte)(type);
					positions[counter++] = { x + 1, y, z };
					types[counter - 1] = (GLbyte)(type);
					positions[counter++] = { x + 1, y, z + 1 };
					types[counter - 1] = (GLbyte)(type);
					positions[counter++] = { x + 1, y, z + 1 };
					types[counter - 1] = (GLbyte)(type);
					positions[counter++] = { x, y, z + 1 };
					types[counter - 1] = (GLbyte)(type);
					positions[counter++] = { x, y, z };
					types[counter - 1] = (GLbyte)(type);
				}
			}
		}

		int posBufferSize = CHUNK_X * CHUNK_Y * CHUNK_Z * 6 * 6 * 3 * sizeof(GLbyte);
		int typeBufferSize = CHUNK_X * CHUNK_Y * CHUNK_Z * 6 * 6 * sizeof(GLbyte);

		m_VAO->Bind();
		auto posBuffer = new VertexBuffer(posBufferSize, 3, positions, GL_STATIC_DRAW);
		m_VAO->AddBuffer(posBuffer, 0, GL_BYTE);

		auto typeBuffer = new VertexBuffer(typeBufferSize, 1, types, GL_STATIC_DRAW);
		m_VAO->AddBuffer(typeBuffer, 1, GL_BYTE);
		m_VAO->Unbind();

		m_VertexCount = counter;
	}
}
