#include <glm/glm.hpp>

#include "Chunk.h"
#include "../utils/Logger.h"
#include "ChunkManager.h"

namespace CoreGameObjects
{
	Chunk::Chunk()
		: m_VAO(new VertexArray()), m_Position(0.0f, 0.0f, 0.0f), m_Rebuild(false), m_VertexCount(0)
	{

	}

	Chunk::Chunk(const glm::vec3& position)
		: m_VAO(new VertexArray()), m_Position(position), m_Rebuild(false), m_VertexCount(0)
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

		m_Blocks[x + CHUNK_X * (y + CHUNK_Y * z)] = type;
		m_Rebuild = true;
	}

	BlockType Chunk::GetBlock(int x, int y, int z) const
	{
		if (x >= CHUNK_X || y >= CHUNK_Y || z >= CHUNK_Z
			|| x < 0 || y < 0 || z < 0)
		{
			return BlockType::UNDEFINED;
		}

		return m_Blocks[x + CHUNK_X * (y + CHUNK_Y * z)];
	}

	void Chunk::Build()
	{
		m_Rebuild = false;

		// The position layout contains one extra byte per vertex in order to have 4-byte alignment which is unused.
		// Could probably change around the data types in order to maximize usage

		auto positions = new glm::tvec4<GLshort>[CHUNK_X * CHUNK_Y * CHUNK_Z * 6 * 6]{};
		auto uv = new glm::tvec2<GLfloat>[CHUNK_X * CHUNK_Y * CHUNK_Z * 6 * 6];
		auto types = new GLbyte[CHUNK_X * CHUNK_Y * CHUNK_Z * 6 * 6];
		auto normals = new glm::tvec3<GLfloat>[CHUNK_X * CHUNK_Y * CHUNK_Z * 6 * 6];

		int counter = 0;

		for (int x = 0; x < CHUNK_X; x++)
		{
			for (int y = 0; y < CHUNK_Y; y++)
			{
				for (int z = 0; z < CHUNK_Z; z++)
				{
					auto type = GetBlock(x, y, z);

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

					if (obscuringChunkFront == this)
						obscuringChunkFront = nullptr;
					if (obscuringChunkBack == this)
						obscuringChunkBack = nullptr;
					if (obscuringChunkLeft == this)
						obscuringChunkLeft = nullptr;
					if (obscuringChunkRight == this)
						obscuringChunkRight = nullptr;

					// Incase a bordering chunk exists, check whether its neighboring block is obscuring the current one
					if (obscuringChunkFront)
						obscuringChunkFront = obscuringChunkFront->GetBlock(x, y, 0) == BlockType::AIR ? nullptr : obscuringChunkFront;
					if (obscuringChunkBack)
						obscuringChunkBack = obscuringChunkBack->GetBlock(x, y, CHUNK_Z - 1) == BlockType::AIR ? nullptr : obscuringChunkBack;
					if (obscuringChunkLeft)
						obscuringChunkLeft = obscuringChunkLeft->GetBlock(CHUNK_X - 1, y, z) == BlockType::AIR ? nullptr : obscuringChunkLeft;
					if (obscuringChunkRight)
						obscuringChunkRight = obscuringChunkRight->GetBlock(0, y, z) == BlockType::AIR ? nullptr : obscuringChunkRight;

					// Vertex data.
					// Can probably be shortened in some way (too lazy to find out how atm)
					// Each voxel face is only created if there is no adjacent face obscuring it

					// Front
					if (frontType == BlockType::UNDEFINED && !obscuringChunkFront || frontType == BlockType::AIR)
					{
						positions[counter++] = { x + 1, y + 1, z + 1, 1 };
						types[counter - 1] = (GLbyte)(type);
						uv[counter - 1] = { 1,1 };
						normals[counter - 1] = { 0.0f, 0.0f, 1.0f };

						positions[counter++] = { x, y + 1, z + 1, 1 };
						types[counter - 1] = (GLbyte)(type);
						uv[counter - 1] = { 0, 1 };
						normals[counter - 1] = { 0.0f, 0.0f, 1.0f };

						positions[counter++] = { x, y, z + 1, 1 };
						types[counter - 1] = (GLbyte)(type);
						uv[counter - 1] = { 0, 0 };
						normals[counter - 1] = { 0.0f, 0.0f, 1.0f };

						positions[counter++] = { x, y, z + 1, 1 };
						types[counter - 1] = (GLbyte)(type);
						uv[counter - 1] = { 0, 0 };
						normals[counter - 1] = { 0.0f, 0.0f, 1.0f };

						positions[counter++] = { x + 1, y, z + 1, 1 };
						types[counter - 1] = (GLbyte)(type);
						uv[counter - 1] = { 1, 0 };
						normals[counter - 1] = { 0.0f, 0.0f, 1.0f };

						positions[counter++] = { x + 1, y + 1, z + 1, 1 };
						types[counter - 1] = (GLbyte)(type);
						uv[counter - 1] = { 1, 1 };
						normals[counter - 1] = { 0.0f, 0.0f, 1.0f };
					}

					// Back
					if (backType == BlockType::UNDEFINED && !obscuringChunkBack || backType == BlockType::AIR)
					{
						positions[counter++] = { x, y + 1, z, 1 };
						types[counter - 1] = (GLbyte)(type);
						uv[counter - 1] = { 1, 1 };
						normals[counter - 1] = { 0.0f, 0.0f, -1.0f };

						positions[counter++] = { x + 1, y + 1, z, 1 };
						types[counter - 1] = (GLbyte)(type);
						uv[counter - 1] = { 0, 1};
						normals[counter - 1] = { 0.0f, 0.0f, -1.0f };

						positions[counter++] = { x + 1, y, z, 1 };
						types[counter - 1] = (GLbyte)(type);
						uv[counter - 1] = { 0, 0 };
						normals[counter - 1] = { 0.0f, 0.0f, -1.0f };

						positions[counter++] = { x + 1, y, z, 1 };
						types[counter - 1] = (GLbyte)(type);
						uv[counter - 1] = { 0, 0 };
						normals[counter - 1] = { 0.0f, 0.0f, -1.0f };

						positions[counter++] = { x, y, z, 1 };
						types[counter - 1] = (GLbyte)(type);
						uv[counter - 1] = { 1, 0 };
						normals[counter - 1] = { 0.0f, 0.0f, -1.0f };

						positions[counter++] = { x, y + 1, z, 1 };
						types[counter - 1] = (GLbyte)(type);
						uv[counter - 1] = { 1, 1 };
						normals[counter - 1] = { 0.0f, 0.0f, -1.0f };
					}

					// Left
					if (leftType == BlockType::UNDEFINED && !obscuringChunkLeft || leftType == BlockType::AIR)
					{
						positions[counter++] = { x, y + 1, z + 1, 1 };
						types[counter - 1] = (GLbyte)(type);
						uv[counter - 1] = { 1, 1 };
						normals[counter - 1] = { -1.0f, 0.0f, 0.0f };

						positions[counter++] = { x, y + 1, z, 1 };
						types[counter - 1] = (GLbyte)(type);
						uv[counter - 1] = { 0, 1 };
						normals[counter - 1] = { -1.0f, 0.0f, 0.0f };

						positions[counter++] = { x, y, z, 1 };
						types[counter - 1] = (GLbyte)(type);
						uv[counter - 1] = { 0, 0 };
						normals[counter - 1] = { -1.0f, 0.0f, 0.0f };

						positions[counter++] = { x, y, z, 1 };
						types[counter - 1] = (GLbyte)(type);
						uv[counter - 1] = { 0, 0 };
						normals[counter - 1] = { -1.0f, 0.0f, 0.0f };

						positions[counter++] = { x, y, z + 1, 1 };
						types[counter - 1] = (GLbyte)(type);
						uv[counter - 1] = { 1, 0 };
						normals[counter - 1] = { -1.0f, 0.0f, 0.0f };

						positions[counter++] = { x, y + 1, z + 1, 1 };
						types[counter - 1] = (GLbyte)(type);
						uv[counter - 1] = { 1, 1 };
						normals[counter - 1] = { -1.0f, 0.0f, 0.0f };
					}

					// Right
					if (rightType == BlockType::UNDEFINED && !obscuringChunkRight || rightType == BlockType::AIR)
					{
						positions[counter++] = { x + 1, y + 1, z, 1 };
						types[counter - 1] = (GLbyte)(type);
						uv[counter - 1] = { 1, 1 };
						normals[counter - 1] = { 1.0f, 0.0f, 0.0f };

						positions[counter++] = { x + 1, y + 1, z + 1, 1 };
						types[counter - 1] = (GLbyte)(type);
						uv[counter - 1] = { 0, 1 };
						normals[counter - 1] = { 1.0f, 0.0f, 0.0f };

						positions[counter++] = { x + 1, y, z + 1, 1 };
						types[counter - 1] = (GLbyte)(type);
						uv[counter - 1] = { 0, 0 };
						normals[counter - 1] = { 1.0f, 0.0f, 0.0f };

						positions[counter++] = { x + 1, y, z + 1, 1 };
						types[counter - 1] = (GLbyte)(type);
						uv[counter - 1] = { 0, 0 };
						normals[counter - 1] = { 1.0f, 0.0f, 0.0f };

						positions[counter++] = { x + 1, y, z, 1 };
						types[counter - 1] = (GLbyte)(type);
						uv[counter - 1] = { 1, 0 };
						normals[counter - 1] = { 1.0f, 0.0f, 0.0f };

						positions[counter++] = { x + 1, y + 1, z, 1 };
						types[counter - 1] = (GLbyte)(type);
						uv[counter - 1] = { 1, 1 };
						normals[counter - 1] = { 1.0f, 0.0f, 0.0f };
					}

					// Top
					if (topType== BlockType::UNDEFINED || topType == BlockType::AIR)
					{
						positions[counter++] = { x + 1, y + 1, z, 0 };
						types[counter - 1] = (GLbyte)(type==BlockType::GRASS? (int)type + 1: (int)type);
						uv[counter - 1] = { 1, 1 };
						normals[counter - 1] = { 0.0f, 1.0f, 0.0f };

						positions[counter++] = { x , y + 1, z, 0 };
						types[counter - 1] = (GLbyte)(type == BlockType::GRASS ? (int)type + 1 : (int)type);
						uv[counter - 1] = { 0, 1 };
						normals[counter - 1] = { 0.0f, 1.0f, 0.0f };

						positions[counter++] = { x, y + 1, z + 1, 0 };
						types[counter - 1] = (GLbyte)(type == BlockType::GRASS ? (int)type + 1 : (int)type);
						uv[counter - 1] = { 0, 0 };
						normals[counter - 1] = { 0.0f, 1.0f, 0.0f };

						positions[counter++] = { x, y + 1, z + 1, 0 };
						types[counter - 1] = (GLbyte)(type == BlockType::GRASS ? (int)type + 1 : (int)type);
						uv[counter - 1] = { 0, 0 };
						normals[counter - 1] = { 0.0f, 1.0f, 0.0f };

						positions[counter++] = { x + 1, y + 1, z + 1, 0 };
						types[counter - 1] = (GLbyte)(type == BlockType::GRASS ? (int)type + 1 : (int)type);
						uv[counter - 1] = { 1, 0 };
						normals[counter - 1] = { 0.0f, 1.0f, 0.0f };

						positions[counter++] = { x + 1, y + 1, z, 0 };
						types[counter - 1] = (GLbyte)(type == BlockType::GRASS ? (int)type + 1 : (int)type);
						uv[counter - 1] = { 1, 1 };
						normals[counter - 1] = { 0.0f, 1.0f, 0.0f };
					}

					// Bottom
					if (bottomType == BlockType::UNDEFINED || bottomType == BlockType::AIR)
					{
						positions[counter++] = { x, y, z, 0 };
						types[counter - 1] = (GLbyte)(type == BlockType::GRASS ? (int)type + 2 : (int)type);
						uv[counter - 1] = { 1, 1 };
						normals[counter - 1] = { 0.0f, -1.0f, 0.0f };

						positions[counter++] = { x + 1, y, z, 0 };
						types[counter - 1] = (GLbyte)(type == BlockType::GRASS ? (int)type + 2 : (int)type);;
						uv[counter - 1] = { 0, 1 };
						normals[counter - 1] = { 0.0f, -1.0f, 0.0f };

						positions[counter++] = { x + 1, y, z + 1, 0 };
						types[counter - 1] = (GLbyte)(type == BlockType::GRASS ? (int)type + 2 : (int)type);
						uv[counter - 1] = { 0, 0 };
						normals[counter - 1] = { 0.0f, -1.0f, 0.0f };

						positions[counter++] = { x + 1, y, z + 1, 0 };
						types[counter - 1] = (GLbyte)(type == BlockType::GRASS ? (int)type + 2 : (int)type);
						uv[counter - 1] = { 0, 0 };
						normals[counter - 1] = { 0.0f, -1.0f, 0.0f };

						positions[counter++] = { x, y, z + 1, 0 };
						types[counter - 1] = (GLbyte)(type == BlockType::GRASS ? (int)type + 2 : (int)type);
						uv[counter - 1] = { 1, 0 };
						normals[counter - 1] = { 0.0f, -1.0f, 0.0f };

						positions[counter++] = { x, y, z, 0 };
						types[counter - 1] = (GLbyte)(type == BlockType::GRASS ? (int)type + 2 : (int)type);
						uv[counter - 1] = { 1, 1 };
						normals[counter - 1] = { 0.0f, -1.0f, 0.0f };
					}
				}
			}
		}

		GLuint posBufferSize = CHUNK_X * CHUNK_Y * CHUNK_Z * 6 * 6 * 4 * sizeof(GLshort);
		GLuint uvBufferSize = CHUNK_X * CHUNK_Y * CHUNK_Z * 6 * 6 * 2 * sizeof(GLfloat);
		GLuint typeBufferSize = CHUNK_X * CHUNK_Y * CHUNK_Z * 6 * 6 * sizeof(GLbyte);
		GLuint normalsBufferSize = CHUNK_X * CHUNK_Y * CHUNK_Z * 6 * 6 * 3 * sizeof(GLfloat);

		m_VAO->Bind();
		auto posBuffer = new VertexBuffer(posBufferSize, 4, positions, GL_STATIC_DRAW);
		m_VAO->AddBuffer(posBuffer, 0, GL_SHORT);

		auto uvBuffer = new VertexBuffer(uvBufferSize, 2, uv, GL_STATIC_DRAW);
		m_VAO->AddBuffer(uvBuffer, 1, GL_FLOAT);

		auto typeBuffer = new VertexBuffer(typeBufferSize, 1, types, GL_STATIC_DRAW);
		m_VAO->AddBuffer(typeBuffer, 2, GL_BYTE);

		auto normalsBuffer = new VertexBuffer(normalsBufferSize, 3, normals, GL_STATIC_DRAW);
		m_VAO->AddBuffer(normalsBuffer, 3, GL_FLOAT);
		m_VAO->Unbind();

		m_VertexCount = counter;

		delete[] positions;
		delete[] uv;
		delete[] types;
		delete[] normals;
	}
}
