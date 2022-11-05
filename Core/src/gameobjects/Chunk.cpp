#include "Chunk.h"
#include "../utils/Logger.h"
#include "ChunkManager.h"

namespace CoreGameObjects
{
	Chunk::Chunk()
		: m_VAO(nullptr), m_Position(0.0f, 0.0f, 0.0f),  m_ShouldDispose(false), m_ShouldRender(false), m_IsUploaded(false), m_Built(false), m_VertexCount(0)
	{

	}

	Chunk::Chunk(const glm::vec3& position)
		: m_VAO(nullptr), m_Position(position), m_ShouldDispose(false), m_ShouldRender(false), m_IsUploaded(false), m_Built(false), m_VertexCount(0)
	{

	}

	Chunk::~Chunk()
	{
		delete m_VAO;
	}

	GLushort PackIntoShort(GLushort val1, GLushort val2)
	{
		GLushort packedValue = 0;

		packedValue |= val1;
		packedValue |= (val2 << 8);

		return packedValue;
	}

	void Chunk::EraseBuffers()
	{
		// Reinitializes all of the buffer vectors with empty ones, thereby emptying them and releasing any reserved memory

		m_Buffers.positions = std::vector<glm::tvec4<GLshort>>();
		m_Buffers.uv = std::vector<GLushort>();
		m_Buffers.types = std::vector<GLbyte>();
	}

	void Chunk::SetBlock(int x, int y, int z, BlockType type)
	{
		if (x >= CHUNK_X || y >= CHUNK_Y || z >= CHUNK_Z
			|| x < 0 || y < 0 || z < 0)
		{
			LOG_ERROR("Cannot set block to out of bounds!");
			return;
		}

		m_Blocks[x + CHUNK_X * (y + CHUNK_Y * z)] = (signed char)type;
	}

	BlockType Chunk::GetBlock(int x, int y, int z) const
	{
		if (x >= CHUNK_X || y >= CHUNK_Y || z >= CHUNK_Z
			|| x < 0 || y < 0 || z < 0)
		{
			return BlockType::UNDEFINED;
		}

		return (BlockType)m_Blocks[x + CHUNK_X * (y + CHUNK_Y * z)];
	}

	void Chunk::Build(bool rebuild)
	{
		if (!rebuild)
			FindObscuringChunks();

		// Immediately reserving enough space for 65000 vertices (a rough estimate) in order to prevent reallocation of the vectors at every insertion
		m_Buffers.positions.reserve(65000);
		m_Buffers.uv.reserve(65000);
		m_Buffers.types.reserve(65000);

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

					bool obscuredFront = false;
					bool obscuredBack = false;
					bool obscuredLeft = false;
					bool obscuredRight = false;

					// Incase a bordering chunk exists, check whether its neighboring block is obscuring the current one
					if (m_ObscuringChunks[0])
						obscuredFront = m_ObscuringChunks[0]->GetBlock(x, y, 0) == BlockType::AIR ? false : true;
					if (m_ObscuringChunks[1])
						obscuredBack = m_ObscuringChunks[1]->GetBlock(x, y, CHUNK_Z - 1) == BlockType::AIR ? false : true;
					if (m_ObscuringChunks[2])
						obscuredLeft = m_ObscuringChunks[2]->GetBlock(CHUNK_X - 1, y, z) == BlockType::AIR ? false : true;
					if (m_ObscuringChunks[3])
						obscuredRight = m_ObscuringChunks[3]->GetBlock(0, y, z) == BlockType::AIR ? false : true;

					// Vertex data.
					// Can probably be shortened in some way (too lazy to find out how atm)
					// Each voxel face is only created if there is no adjacent face obscuring it

					// Front
					if (frontType == BlockType::UNDEFINED && !obscuredFront || frontType == BlockType::AIR)
					{
						m_Buffers.positions.emplace_back(x + 1, y + 1, z + 1, 0);
						m_Buffers.uv.emplace_back(PackIntoShort(1, 1));
						m_Buffers.types.emplace_back((GLbyte)type);
						
						m_Buffers.positions.emplace_back( x, y + 1, z + 1, 0);
						m_Buffers.uv.emplace_back(PackIntoShort(0, 1));
						m_Buffers.types.emplace_back((GLbyte)type);
						
						m_Buffers.positions.emplace_back(x, y, z + 1, 0);
						m_Buffers.uv.emplace_back(PackIntoShort(0, 0));
						m_Buffers.types.emplace_back((GLbyte)type);
						
						m_Buffers.positions.emplace_back(x, y, z + 1, 0);
						m_Buffers.uv.emplace_back(PackIntoShort(0, 0));
						m_Buffers.types.emplace_back((GLbyte)type);
						
						m_Buffers.positions.emplace_back(x + 1, y, z + 1, 0);
						m_Buffers.uv.emplace_back(PackIntoShort(1, 0));
						m_Buffers.types.emplace_back((GLbyte)type);
						
						m_Buffers.positions.emplace_back(x + 1, y + 1, z + 1, 0);
						m_Buffers.uv.emplace_back(PackIntoShort(1, 1));
						m_Buffers.types.emplace_back((GLbyte)type);
					}

					// Back
					if (backType == BlockType::UNDEFINED && !obscuredBack || backType == BlockType::AIR)
					{
						m_Buffers.positions.emplace_back(x, y + 1, z, 1);
						m_Buffers.uv.emplace_back(PackIntoShort(1, 1));
						m_Buffers.types.emplace_back((GLbyte)type);
						
						m_Buffers.positions.emplace_back(x + 1, y + 1, z, 1);
						m_Buffers.uv.emplace_back(PackIntoShort(0, 1));
						m_Buffers.types.emplace_back((GLbyte)type);
						
						m_Buffers.positions.emplace_back(x + 1, y, z, 1);
						m_Buffers.uv.emplace_back(PackIntoShort(0, 0));
						m_Buffers.types.emplace_back((GLbyte)type);
						
						m_Buffers.positions.emplace_back(x + 1, y, z, 1);
						m_Buffers.uv.emplace_back(PackIntoShort(0, 0));
						m_Buffers.types.emplace_back((GLbyte)type);
						
						m_Buffers.positions.emplace_back(x, y, z, 1);
						m_Buffers.uv.emplace_back(PackIntoShort(1, 0));
						m_Buffers.types.emplace_back((GLbyte)type);
						
						m_Buffers.positions.emplace_back(x, y + 1, z, 1);
						m_Buffers.uv.emplace_back(PackIntoShort(1, 1));
						m_Buffers.types.emplace_back((GLbyte)type);
					}

					// Left
					if (leftType == BlockType::UNDEFINED && !obscuredLeft || leftType == BlockType::AIR)
					{
						m_Buffers.positions.emplace_back(x, y + 1, z + 1, 2);
						m_Buffers.uv.emplace_back(PackIntoShort(1, 1));
						m_Buffers.types.emplace_back((GLbyte)type);
						
						m_Buffers.positions.emplace_back(x, y + 1, z, 2);
						m_Buffers.uv.emplace_back(PackIntoShort(0, 1));
						m_Buffers.types.emplace_back((GLbyte)type);
						
						m_Buffers.positions.emplace_back(x, y, z, 2);
						m_Buffers.uv.emplace_back(PackIntoShort(0, 0));
						m_Buffers.types.emplace_back((GLbyte)type);
						
						m_Buffers.positions.emplace_back(x, y, z, 2);
						m_Buffers.uv.emplace_back(PackIntoShort(0, 0));
						m_Buffers.types.emplace_back((GLbyte)type);
						
						m_Buffers.positions.emplace_back(x, y, z + 1, 2);
						m_Buffers.uv.emplace_back(PackIntoShort(1, 0));
						m_Buffers.types.emplace_back((GLbyte)type);

						m_Buffers.positions.emplace_back(x, y + 1, z + 1, 2);
						m_Buffers.uv.emplace_back(PackIntoShort(1, 1));
						m_Buffers.types.emplace_back((GLbyte)type);
					}

					// Right
					if (rightType == BlockType::UNDEFINED && !obscuredRight || rightType == BlockType::AIR)
					{
						m_Buffers.positions.emplace_back(x + 1, y + 1, z, 3);
						m_Buffers.uv.emplace_back(PackIntoShort(1, 1));
						m_Buffers.types.emplace_back((GLbyte)type);
						
						m_Buffers.positions.emplace_back(x + 1, y + 1, z + 1, 3);
						m_Buffers.uv.emplace_back(PackIntoShort(0, 1));
						m_Buffers.types.emplace_back((GLbyte)type);
						
						m_Buffers.positions.emplace_back(x + 1, y, z + 1, 3);
						m_Buffers.uv.emplace_back(PackIntoShort(0, 0));
						m_Buffers.types.emplace_back((GLbyte)type);
						
						m_Buffers.positions.emplace_back(x + 1, y, z + 1, 3);
						m_Buffers.uv.emplace_back(PackIntoShort(0, 0));
						m_Buffers.types.emplace_back((GLbyte)type);
						
						m_Buffers.positions.emplace_back(x + 1, y, z, 3);
						m_Buffers.uv.emplace_back(PackIntoShort(1, 0));
						m_Buffers.types.emplace_back((GLbyte)type);
						
						m_Buffers.positions.emplace_back(x + 1, y + 1, z, 3);
						m_Buffers.uv.emplace_back(PackIntoShort(1, 1));
						m_Buffers.types.emplace_back((GLbyte)type);
					}

					// Top
					if (topType== BlockType::UNDEFINED || topType == BlockType::AIR)
					{
						m_Buffers.positions.emplace_back(x + 1, y + 1, z, 4);
						m_Buffers.uv.emplace_back(PackIntoShort(1, 1));
						m_Buffers.types.emplace_back((GLbyte)(type==BlockType::GRASS? (int)type + 1: (int)type));
						
						m_Buffers.positions.emplace_back(x , y + 1, z, 4);
						m_Buffers.uv.emplace_back(PackIntoShort(0, 1));
						m_Buffers.types.emplace_back((GLbyte)(type==BlockType::GRASS? (int)type + 1: (int)type));

						m_Buffers.positions.emplace_back(x, y + 1, z + 1, 4);
						m_Buffers.uv.emplace_back(PackIntoShort(0, 0));
						m_Buffers.types.emplace_back((GLbyte)(type==BlockType::GRASS? (int)type + 1: (int)type));
						
						m_Buffers.positions.emplace_back(x, y + 1, z + 1, 4);
						m_Buffers.uv.emplace_back(PackIntoShort(0, 0));
						m_Buffers.types.emplace_back((GLbyte)(type==BlockType::GRASS? (int)type + 1: (int)type));
						
						m_Buffers.positions.emplace_back(x + 1, y + 1, z + 1, 4);
						m_Buffers.uv.emplace_back(PackIntoShort(1, 0));
						m_Buffers.types.emplace_back((GLbyte)(type==BlockType::GRASS? (int)type + 1: (int)type));
						
						m_Buffers.positions.emplace_back(x + 1, y + 1, z, 4);
						m_Buffers.uv.emplace_back(PackIntoShort(1, 1));
						m_Buffers.types.emplace_back((GLbyte)(type==BlockType::GRASS? (int)type + 1: (int)type));
					}

					// Bottom
					if (bottomType == BlockType::UNDEFINED || bottomType == BlockType::AIR)
					{
						m_Buffers.positions.emplace_back(x, y, z, 5);
						m_Buffers.uv.emplace_back(PackIntoShort(1, 1));
						m_Buffers.types.emplace_back((GLbyte)(type == BlockType::GRASS ? (int)type + 2 : (int)type));
						
						m_Buffers.positions.emplace_back(x + 1, y, z, 5);
						m_Buffers.uv.emplace_back(PackIntoShort(0, 1));
						m_Buffers.types.emplace_back((GLbyte)(type == BlockType::GRASS ? (int)type + 2 : (int)type));
						
						m_Buffers.positions.emplace_back(x + 1, y, z + 1, 5);
						m_Buffers.uv.emplace_back(PackIntoShort(0, 0));
						m_Buffers.types.emplace_back((GLbyte)(type == BlockType::GRASS ? (int)type + 2 : (int)type));
						
						m_Buffers.positions.emplace_back(x + 1, y, z + 1, 5);
						m_Buffers.uv.emplace_back(PackIntoShort(0, 0));
						m_Buffers.types.emplace_back((GLbyte)(type == BlockType::GRASS ? (int)type + 2 : (int)type));
						
						m_Buffers.positions.emplace_back(x, y, z + 1, 5);
						m_Buffers.uv.emplace_back(PackIntoShort(1, 0));
						m_Buffers.types.emplace_back((GLbyte)(type == BlockType::GRASS ? (int)type + 2 : (int)type));
						
						m_Buffers.positions.emplace_back(x, y, z, 5);
						m_Buffers.uv.emplace_back(PackIntoShort(1, 1));
						m_Buffers.types.emplace_back((GLbyte)(type == BlockType::GRASS ? (int)type + 2 : (int)type));
					}
				}
			}
		}

		m_VertexCount = m_Buffers.positions.size();
		m_Built = true;
	}

	void Chunk::UploadToGPU()
	{
		m_VAO = new VertexArray();

		GLuint posBufferSize = m_Buffers.positions.size() * 4 * sizeof(GLshort);
		GLuint uvBufferSize = m_Buffers.uv.size() * sizeof(GLushort);
		GLuint typeBufferSize = m_Buffers.types.size() * sizeof(GLbyte);

		m_VAO->Bind();

		auto posBuffer = new VertexBuffer(posBufferSize, 4);
		posBuffer->BufferData(m_Buffers.positions.data(), GL_STATIC_DRAW);
		m_VAO->AddBuffer(posBuffer, 0, GL_SHORT);

		auto uvBuffer = new VertexBuffer(uvBufferSize, 1);
		uvBuffer->BufferData(m_Buffers.uv.data(), GL_STATIC_DRAW);
		m_VAO->AddBuffer(uvBuffer, 1, GL_UNSIGNED_SHORT);

		auto typeBuffer = new VertexBuffer(typeBufferSize, 1);
		typeBuffer->BufferData(m_Buffers.types.data(), GL_STATIC_DRAW);
		m_VAO->AddBuffer(typeBuffer, 2, GL_BYTE);

		m_VAO->Unbind();

		EraseBuffers();
		m_IsUploaded = true;
	}

	void Chunk::UpdateGPUData()
	{
		m_VAO->Bind();

		GLuint posBufferSize = m_Buffers.positions.size() * 4 * sizeof(GLshort);
		GLuint uvBufferSize = m_Buffers.uv.size() * 2 * sizeof(GLfloat);
		GLuint typeBufferSize = m_Buffers.types.size() * sizeof(GLbyte);

		m_VAO->GetBuffer(0)->BufferSubData(0, posBufferSize, m_Buffers.positions.data());
		m_VAO->GetBuffer(1)->BufferSubData(0, uvBufferSize, m_Buffers.uv.data());
		m_VAO->GetBuffer(2)->BufferSubData(0, typeBufferSize, m_Buffers.types.data());

		m_VAO->Unbind();

		m_VertexCount -= m_VertexCount - posBufferSize;

		EraseBuffers();
	}

	void Chunk::FindObscuringChunks()
	{
		for (auto& loadedChunk : ChunkManager::GetLoadedChunks())
		{
			if (loadedChunk.get() == this || loadedChunk.get() == nullptr)
				continue;

			auto& chunkPos = loadedChunk->GetPos();

			if (chunkPos.x == m_Position.x && chunkPos.z == m_Position.z + CHUNK_Z) // Front
			{
				loadedChunk.get()->SetObscuring(1, this);
				m_ObscuringChunks[0] = loadedChunk.get();
			}
			else if (chunkPos.x == m_Position.x && chunkPos.z == m_Position.z - CHUNK_Z) // Back
			{
				loadedChunk.get()->SetObscuring(0, this);
				m_ObscuringChunks[1] = loadedChunk.get();
			}
			else if (chunkPos.x == m_Position.x - CHUNK_X && chunkPos.z == m_Position.z) // Left
			{
				loadedChunk.get()->SetObscuring(3, this);
				m_ObscuringChunks[2] = loadedChunk.get();
			}
			else if (chunkPos.x == m_Position.x + CHUNK_X && chunkPos.z == m_Position.z) // Right
			{
				loadedChunk.get()->SetObscuring(2, this);
				m_ObscuringChunks[3] = loadedChunk.get();
			}
		}
	}
}
