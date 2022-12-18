#include "PCH.h"

#include "Chunk.h"
#include "ChunkManager.h"
#include "../graphics/VertexArrayManager.h"
#include "../utils/Dashboard.h"

namespace CoreGameObjects
{
	using namespace CoreUtils;

	Chunk::Chunk()
		: m_VAO(nullptr), m_Position(0.0f, 0.0f, 0.0f),  m_ShouldDispose(false), m_ShouldRender(false), m_IsUploaded(false), m_Built(false), m_Serialized(true), m_Visible(false), m_VertexCount(0)
	{
		m_Model = glm::translate(glm::identity<glm::mat4>(), m_Position);
	}

	Chunk::Chunk(const glm::vec3& position)
		: m_VAO(nullptr), m_Position(position), m_ShouldDispose(false), m_ShouldRender(false), m_IsUploaded(false), m_Built(false), m_Serialized(true), m_Visible(false),  m_VertexCount(0)
	{
		m_Model = glm::translate(glm::identity<glm::mat4>(), m_Position);
	}

	Chunk::~Chunk()
	{

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

		m_Buffers.positions = std::vector<glm::tvec4<GLbyte>>();
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

		m_Blocks[x * CHUNK_Y * CHUNK_X + y * CHUNK_X + z] = (signed char)type;
	}

	void Chunk::SetPosition(const glm::vec3& position)
	{
		m_Position = position;
		m_Model = glm::translate(glm::identity<glm::mat4>(), m_Position);
	}

	BlockType Chunk::GetBlock(int x, int y, int z) const
	{
		if (x >= CHUNK_X || y >= CHUNK_Y || z >= CHUNK_Z
			|| x < 0 || y < 0 || z < 0)
		{
			return BlockType::UNDEFINED;
		}

		return (BlockType)m_Blocks[x * CHUNK_Y * CHUNK_X + y * CHUNK_X + z];
	}

	void Chunk::Build(bool rebuild)
	{
		if (!rebuild)
			FindObscuringChunks();

		EraseBuffers();

		// Immediately reserving enough space for 16 * 128 * 16 blocks of the chunk, in order to prevent reallocation of the vector at every insertion
		// 16 * 128 * 16 * 6 * 6 = 1179648
		unsigned int space = CHUNK_X * CHUNK_Y * CHUNK_Z * 6 * 6;
		m_Buffers.positions.reserve(space);
		m_Buffers.uv.reserve(space);
		m_Buffers.types.reserve(space);

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

					 //Incase a bordering chunk exists, check whether its neighboring block is obscuring the current one
					if (m_ObscuringChunks[0] && z == CHUNK_Z - 1)
						obscuredFront = m_ObscuringChunks[0]->GetBlock(x, y, 0) == BlockType::AIR ? false : true;
					if (m_ObscuringChunks[1] && z == 0)
						obscuredBack = m_ObscuringChunks[1]->GetBlock(x, y, CHUNK_Z - 1) == BlockType::AIR ? false : true;
					if (m_ObscuringChunks[2] && x == 0)
						obscuredLeft = m_ObscuringChunks[2]->GetBlock(CHUNK_X - 1, y, z) == BlockType::AIR ? false : true;
					if (m_ObscuringChunks[3] && x == CHUNK_X - 1)
						obscuredRight = m_ObscuringChunks[3]->GetBlock(0, y, z) == BlockType::AIR ? false : true;
					
					bool genFront = frontType == BlockType::UNDEFINED || frontType == BlockType::AIR;
					bool genBack = backType == BlockType::UNDEFINED || backType == BlockType::AIR;
					bool genLeft = leftType == BlockType::UNDEFINED || leftType == BlockType::AIR;
					bool genRight = rightType == BlockType::UNDEFINED || rightType == BlockType::AIR;
					bool genTop = topType == BlockType::UNDEFINED || topType == BlockType::AIR;
					bool genBottom = bottomType == BlockType::UNDEFINED || bottomType == BlockType::AIR;

					if (obscuredFront)
						genFront = false;
					if (obscuredBack)
						genBack = false;
					if (obscuredLeft)
						genLeft = false;
					if (obscuredRight)
						genRight = false;

					// Vertex data.
					// Can probably be shortened in some way (too lazy to find out how atm)
					// Each voxel face is only created if there is no adjacent face obscuring it

					// Front
					if (genFront)
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
					if (genBack)
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
					if (genLeft)
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
					if (genRight)
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
					if (genTop)
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
					if (genBottom)
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
		if (!VertexArrayManager::IsEmpty())
		{
			UpdateGPUData();
			return;
		}

		m_VAO = new VertexArray();

		GLuint posBufferSize = CHUNK_X * CHUNK_Y * CHUNK_Z * 6 * 6 * 4 * sizeof(GLbyte);
		GLuint uvBufferSize = CHUNK_X * CHUNK_Y * CHUNK_Z * 6 * 6 * sizeof(GLshort);
		GLuint typeBufferSize = CHUNK_X * CHUNK_Y * CHUNK_Z * 6 * 6 * sizeof(GLbyte);

		m_VAO->Bind();

		auto posBuffer = new VertexBuffer(posBufferSize, 4);
		posBuffer->BufferData(m_Buffers.positions.data(), GL_STREAM_DRAW);
		m_VAO->AddBuffer(posBuffer, 0, GL_BYTE);

		auto uvBuffer = new VertexBuffer(uvBufferSize, 1);
		uvBuffer->BufferData(m_Buffers.uv.data(), GL_STREAM_DRAW);
		m_VAO->AddBuffer(uvBuffer, 1, GL_UNSIGNED_SHORT);

		auto typeBuffer = new VertexBuffer(typeBufferSize, 1);
		typeBuffer->BufferData(m_Buffers.types.data(), GL_STREAM_DRAW);
		m_VAO->AddBuffer(typeBuffer, 2, GL_BYTE);

		m_VAO->Unbind();

		m_IsUploaded = true;
	}

	void Chunk::UpdateGPUData()
	{
		if (m_VAO == nullptr)
			m_VAO = VertexArrayManager::GetVAO();

		m_VAO->Bind();

		GLuint posBufferSize = CHUNK_X * CHUNK_Y * CHUNK_Z * 6 * 6 * 4 * sizeof(GLshort);
		GLuint uvBufferSize = CHUNK_X * CHUNK_Y * CHUNK_Z * 6 * 6 * sizeof(GLshort);
		GLuint typeBufferSize = CHUNK_X * CHUNK_Y * CHUNK_Z * 6 * 6 * sizeof(GLbyte);

		m_VAO->GetBuffer(0)->BufferSubData(0, posBufferSize, m_Buffers.positions.data());
		m_VAO->GetBuffer(1)->BufferSubData(0, uvBufferSize, m_Buffers.uv.data());
		m_VAO->GetBuffer(2)->BufferSubData(0, typeBufferSize, m_Buffers.types.data());

		m_VAO->Unbind();

		m_VertexCount = m_Buffers.positions.size();

		m_IsUploaded = true;
	}

	void Chunk::FindObscuringChunks()
	{
		for (auto& loadedChunk : ChunkManager::GetLoadedChunks())
		{
			bool foundNeighbor = false;

			if (loadedChunk.get() == this || loadedChunk.get() == nullptr)
				continue;

			auto chunkPos = loadedChunk->GetPos();

			if (chunkPos.x == m_Position.x && chunkPos.z == m_Position.z + CHUNK_Z) // Front
			{
				loadedChunk.get()->SetObscuring(1, this);
				m_ObscuringChunks[0] = loadedChunk.get();
				foundNeighbor = true;
			}
			else if (chunkPos.x == m_Position.x && chunkPos.z == m_Position.z - CHUNK_Z) // Back
			{
				loadedChunk.get()->SetObscuring(0, this);
				m_ObscuringChunks[1] = loadedChunk.get();
				foundNeighbor = true;
			}
			else if (chunkPos.x == m_Position.x - CHUNK_X && chunkPos.z == m_Position.z) // Left
			{
				loadedChunk.get()->SetObscuring(3, this);
				m_ObscuringChunks[2] = loadedChunk.get();
				foundNeighbor = true;
			}
			else if (chunkPos.x == m_Position.x + CHUNK_X && chunkPos.z == m_Position.z) // Right
			{
				loadedChunk.get()->SetObscuring(2, this);
				m_ObscuringChunks[3] = loadedChunk.get();
				foundNeighbor = true;
			}

			//if (foundNeighbor)
			//{
			//	if (loadedChunk->IsBuilt() && !loadedChunk->IsUploaded() && !ChunkManager::IsPositionQueuedForBuild(chunkPos))
			//	{
			//		ChunkManager::QueueForBuild(loadedChunk.get(), true);
			//	}
			//}
		}
	}

	void Chunk::CheckVisibility(const Camera& camera)
	{
		if (!Dashboard::ShouldFrustumCull())
		{
			m_Visible = true;
			return;
		}

		glm::vec3 center = glm::vec3(m_Position.x + CHUNK_X / 2.0f, CHUNK_Y / 2.0f, m_Position.z + CHUNK_Z / 2.0f);

		// We multiply the center coordinate of the chunk with the MVP in order to figure out where the chunk center would be on the screen
		glm::vec4 coordinates = camera.GetViewProjection() * GetModel() * glm::vec4(center, 1.0f);
		// Dividing X and Y by W, we get those coordinates in NDC
		coordinates.x /= coordinates.w;
		coordinates.y /= coordinates.w;

		// The diameter of a sphere that could encompass an entire chunk
		float diameter = std::sqrtf(CHUNK_X * CHUNK_X +  CHUNK_Y * CHUNK_Y + CHUNK_Z * CHUNK_Z);

		if (coordinates.z < -diameter)
		{
			m_Visible = false;
			return;
		}

		// We turn the diameter into the same coordinate space as X and Y
		diameter /= std::fabsf(coordinates.w);

		if (std::fabsf(coordinates.x) > 1 + diameter || fabsf(coordinates.y) > 1 + diameter)
		{
			m_Visible = false;
			return;
		}
			

		m_Visible = true;
	}
}
;