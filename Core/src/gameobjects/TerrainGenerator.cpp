
#include "TerrainGenerator.h"
#include "../utils/Logger.h"
#include "../utils/Maths.h"
#include "ChunkManager.h"

#define MAX_SEED 9999999999
#define MIN_SEED 1

namespace CoreGameObjects
{
	unsigned long long TerrainGenerator::m_Seed = 0;
	double TerrainGenerator::m_LerpedSeed = 0.0;
	Camera* TerrainGenerator::m_Camera = nullptr;

	TerrainGenerator::TerrainGenerator(unsigned long long seed)
		: m_SerializationCount(8), m_RenderDistance(64.0f)
	{
		m_Seed = seed;

		if (m_Seed == 0)
			m_Seed = GetRand<unsigned long long>(1000000000, MAX_SEED);
		else if (m_Seed > MAX_SEED)
		{
			LOG_ERROR("Seed is disallowed, setting a random seed instead");
			m_Seed = GetRand<unsigned long long>(1000000000, MAX_SEED);
		}

		m_LerpedSeed = Interpolate(m_Seed, MIN_SEED, MAX_SEED, -255.0f, 255.0f);

		LOG_INFO("World seed: " << m_Seed);
		LOG_INFO("World seed after interpolation: " << m_LerpedSeed);
	}

	void TerrainGenerator::Init()
	{
		CheckIfGenerated();
		ChunkManager::GetLoadedChunks().reserve(100);
	}

	void TerrainGenerator::LoadChunks()
	{
		PrepareChunks();

		for (auto& loadedChunk : ChunkManager::GetLoadedChunks())
		{
			if (loadedChunk->IsBuilt() && !loadedChunk->IsUploaded())
			{
				loadedChunk->UploadToGPU();
				LOG_INFO("Chunk uploaded to GPU");
			}
		}

		SynchronizeChunks();
	}

	void TerrainGenerator::SynchronizeChunks()
	{
		while (!ChunkManager::GetQueuedForBuild().empty() && ChunkManager::IsBuildQueueReady())
		{
			auto& future = ChunkManager::GetQueuedForBuild().front();
			auto chunk = future.get();
			ChunkManager::LoadChunk(chunk);
			ChunkManager::MarkPositionAsBuilt(chunk->GetPos());
			ChunkManager::GetQueuedForBuild().pop();
		}

		for (auto loadedIt = ChunkManager::GetLoadedChunks().begin(); loadedIt != ChunkManager::GetLoadedChunks().end(); )
		{
			auto chunkPos = loadedIt->get()->GetPos();
			glm::vec2 distanceVector = m_Camera->GetPosition().xz - glm::vec2(chunkPos.x + CHUNK_X, chunkPos.z + CHUNK_Z);
			auto distance = glm::length(distanceVector);

			if (distance > m_RenderDistance && loadedIt->get()->ShouldRender())
			{
				loadedIt->get()->SetShouldRender(false);
				LOG_INFO("Marked chunk for unloading");
			}

			if (chunkPos.x > (float)xBuildBoundaries.y * CHUNK_X || chunkPos.x < (float)xBuildBoundaries.x * CHUNK_X || chunkPos.z > (float)zBuildBoundaries.y * CHUNK_Z || chunkPos.z < (float)zBuildBoundaries.x * CHUNK_Z)
			{
				ChunkManager::SynchronizeObscured(loadedIt->get());
				loadedIt = ChunkManager::GetLoadedChunks().erase(loadedIt);
				LOG_INFO("Disposed chunk");
			}
			else
				++loadedIt;
		}
	}

	bool TerrainGenerator::CheckIfGenerated()
	{
		if (!std::filesystem::is_directory(WRITE_PATH))
		{
			std::filesystem::create_directory(WRITE_PATH);
			LOG_INFO("No chunk folder found, created new one");
			return false;
		}

		size_t fileCount = std::distance(std::filesystem::directory_iterator(WRITE_PATH), std::filesystem::directory_iterator());

		if (fileCount == 0)
			return false;

		LOG_INFO("Found chunk folder with " << fileCount << " chunks");

		return true;
	}

	void TerrainGenerator::PrepareChunks()
	{
		const glm::vec3 cameraPos = m_Camera->GetPosition();
 		glm::vec2 cameraPosInChunks = {cameraPos.x / 16, cameraPos.z / 16};

		xBuildBoundaries.x = cameraPosInChunks.x - (m_SerializationCount / 2);
		xBuildBoundaries.y = cameraPosInChunks.x + (m_SerializationCount / 2);

		zBuildBoundaries.x = cameraPosInChunks.y - (m_SerializationCount / 2);
		zBuildBoundaries.y = cameraPosInChunks.y + (m_SerializationCount / 2);


		for (int z = zBuildBoundaries.x; z <= zBuildBoundaries.y; z++)
		{
			for (int x = xBuildBoundaries.x; x <= xBuildBoundaries.y; x++)
			{
				glm::vec3 chunkPos = {x * CHUNK_X, 0.0f, z * CHUNK_Z};
				glm::vec2 distanceVector = m_Camera->GetPosition().xz - glm::vec2(chunkPos.x + CHUNK_X, chunkPos.z + CHUNK_Z);
				auto distance = glm::length(distanceVector);

				if (!ChunkManager::IsLoaded(chunkPos) && !ChunkManager::IsPositionQueuedForBuild(chunkPos))
				{
					if (ChunkManager::IsSerialized(chunkPos))
						ChunkManager::GetQueuedForBuild().push(std::async(std::launch::async, ChunkManager::GenerateChunk, chunkPos, true));
					else
						ChunkManager::GetQueuedForBuild().push(std::async(std::launch::async, ChunkManager::GenerateChunk, chunkPos, false));
					
					ChunkManager::MarkPositionForBuild(chunkPos);

					LOG_INFO("Queued chunk for building");
				}
				
				if (distance <= m_RenderDistance && ChunkManager::IsLoaded(chunkPos))
				{
					auto chunk = ChunkManager::GetLoadedChunk(chunkPos);

					if (!chunk->ShouldRender())
					{
						chunk->SetShouldRender(true);
						LOG_INFO("Marked chunk for rendering")
					}
				}
			}
		}
	}

	void TerrainGenerator::Noisify(Chunk& chunk)
	{
		m_LerpedSeed = Interpolate(m_Seed, MIN_SEED, MAX_SEED, -255.0f, 255.0f);
		glm::vec3 chunkPos = chunk.GetPos();
		const float increment = 1000.0f;

		for (int x = 0; x < CHUNK_X; x++)
		{
			for (int z = 0; z < CHUNK_Z; z++)
			{
				int16_t surfaceY = 256;
				int16_t stoneY = 250;

				// Getting block coordinates in world space and adding a seed value lerped between -255.0 and 255.0
				float blockX = x + chunkPos.x + m_LerpedSeed;
				float blockZ = z + chunkPos.z + m_LerpedSeed;

				// The coordinates are divided by an increment to get smaller steps
				float height = Noise(glm::vec2(blockX / increment, blockZ / increment), 7, 0.9f);
				float stoneHeight = Noise(glm::vec2(blockX / increment, blockZ / increment), 7, 1.0f);
				surfaceY += height * 15.0f;
				stoneY += stoneHeight * 20.0f;

				for (int y = 0; y < CHUNK_Y; y++)
				{
					if (y <= stoneY)
						chunk.SetBlock(x, y, z, BlockType::STONE);
					else if (y == surfaceY - 1)
						chunk.SetBlock(x, y, z, BlockType::GRASS);
					else if (y < surfaceY)
						chunk.SetBlock(x, y, z, BlockType::DIRT);
					else
						chunk.SetBlock(x, y, z, BlockType::AIR);
				}
			}
		}
	}

	template <typename Numeral>
	Numeral TerrainGenerator::GetRand(Numeral start, Numeral end)
	{
		thread_local static std::mt19937 generator(std::random_device{}());

		// Switch between integer or real distribution of numbers based on whether the method should return an integer or a float/double
		using distType = typename std::conditional<
			std::is_integral<Numeral>::value, std::uniform_int_distribution<Numeral>, std::uniform_real_distribution<Numeral>
		>::type;

		thread_local static distType distribution;

		return distribution(generator, typename distType::param_type{ start, end });
	}

	float TerrainGenerator::Noise(const glm::vec2& coordinates, int octaves, float persistence)
	{
		float sum = 0.0f;
		float strength = 1.0f;
		float scale = 1.0f;

		for (int i = 0; i < octaves; i++)
		{
			sum += strength * glm::perlin(coordinates * scale);
			scale *= 2.0f;
			strength *= persistence;
		}

		return sum;
	}

	float TerrainGenerator::Noise(const glm::vec3& coordinates, int octaves, float persistence)
	{
		float sum = 0.0f;
		float strength = 1.0f;
		float scale = 1.0f;

		for (int i = 0; i < octaves; i++)
		{
			sum += strength * fabs(glm::perlin(coordinates * scale));
			scale *= 2.0f;
			strength *= persistence;
		}

		return sum;
	}
}
