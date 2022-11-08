
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
		auto camPos = m_Camera->GetPosition();
		auto camPosChunks = glm::vec2(camPos.x / 16, camPos.z / 16);

		int startX = camPosChunks.x - 8;
		int endX = camPosChunks.x + 8;
		int startZ = camPosChunks.y - 8;
		int endZ = camPosChunks.y + 8;

		for (int i = startX; i <= endX; i++)
		{
			for (int j = startZ; j <= endZ; j++)
			{
				auto chunkPos = glm::vec3(i * CHUNK_X, 0.0f, j * CHUNK_Z);
				auto distanceVector = camPos.xz - glm::vec2(chunkPos.x + CHUNK_X, chunkPos.z + CHUNK_Z);
				auto distance = glm::length(distanceVector);

				if (distance <= 128.0f && !ChunkManager::IsLoaded(chunkPos))
				{
					std::shared_ptr<Chunk> chunk;

					if (ChunkManager::IsSerialized(chunkPos))
					{
						chunk = ChunkManager::Deserialize(chunkPos, m_Seed);
						chunk->SetSerialized(true);
					}
					else
					{
						chunk = std::make_shared<Chunk>(chunkPos);
						chunk->SetSerialized(false);
					}

					ChunkManager::LoadChunk(chunk);
					LOG_INFO("Loaded chunk -> " << ChunkManager::GetLoadedChunks().size());
				}
			}
		}

		int counter = 0;

		for (auto& loadedChunk : ChunkManager::GetLoadedChunks())
		{
			if (counter == 4)
				break;

			if (ChunkManager::IsPositionQueuedForBuild(loadedChunk->GetPos()) || loadedChunk->IsBuilt())
				continue;

			ChunkManager::GetQueuedForBuild().push_back(std::async(std::launch::async, ChunkManager::BuildChunk, loadedChunk.get()));
			ChunkManager::MarkPositionForBuild(loadedChunk->GetPos());
			counter++;
		}

		SynchronizeChunks();
	}

	void TerrainGenerator::SynchronizeChunks()
	{
		while (!ChunkManager::GetQueuedForBuild().empty() && ChunkManager::IsBuildQueueReady())
			ChunkManager::GetQueuedForBuild().pop_back();

		auto& camPos = m_Camera->GetPosition();

		for (auto loadedIt = ChunkManager::GetLoadedChunks().begin(); loadedIt != ChunkManager::GetLoadedChunks().end(); )
		{
			auto chunkPos = loadedIt->get()->GetPos();
			auto distanceVector = camPos.xz - glm::vec2(chunkPos.x + CHUNK_X, chunkPos.z + CHUNK_Z);
			auto distance = glm::length(distanceVector);

			if (distance <= 64.0f && loadedIt->get()->IsBuilt() && !loadedIt->get()->ShouldRender())
			{
				loadedIt->get()->SetShouldRender(true);
				LOG_INFO("Marked chunk for rendering");
			}
			else if (distance > 64.0f && loadedIt->get()->IsBuilt() && loadedIt->get()->ShouldRender())
			{
				loadedIt->get()->SetShouldRender(false);
				LOG_INFO("Marked chunk to not render");
			}

			if (distance > 128.0f && loadedIt->get()->IsBuilt())
			{
				loadedIt = ChunkManager::GetLoadedChunks().erase(loadedIt);
				LOG_INFO("Unloaded chunk -> " << ChunkManager::GetLoadedChunks().size());
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
		int counter = 0;

		for (auto& chunk : ChunkManager::GetLoadedChunks())
		{
			if (counter == 1)
				break;

			if (chunk->IsBuilt() && chunk->ShouldRender() && !chunk->IsUploaded())
			{
				chunk->UploadToGPU();
				counter++;
				LOG_INFO("Uploaded chunk to GPU");
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
				int16_t surfaceY = 80;
				int16_t stoneY = 74;

				// Getting block coordinates in world space and adding a seed value lerped between -255.0 and 255.0
				float blockX = x + chunkPos.x + m_LerpedSeed;
				float blockZ = z + chunkPos.z + m_LerpedSeed;

				// The coordinates are divided by an increment to get smaller steps
				float height = Noise(glm::vec2(blockX / increment, blockZ / increment), 7, 0.9f);
				float stoneHeight = Noise(glm::vec2(blockX / increment, blockZ / increment), 7, 1.0f);
				surfaceY += height * 5.0f;
				stoneY += stoneHeight * 10.0f;

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
