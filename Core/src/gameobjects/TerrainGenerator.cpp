
#include <iostream>
#include <cmath>
#include <random>
#include <glm/gtc/noise.hpp>
#include <filesystem>
#include <iterator>

#include "TerrainGenerator.h"
#include "../utils/Logger.h"
#include "ChunkManager.h"

#define MAX_SEED 9999999999
#define MIN_SEED 1

#define CHUNKS 16
#define SEA_LEVEL 8

namespace CoreGameObjects
{
	TerrainGenerator::TerrainGenerator(unsigned long long seed)
		: m_Seed(seed)
	{
		if (m_Seed == 0)
			m_Seed = GetRand<unsigned long long>(1000000000, MAX_SEED);
		else if (m_Seed > MAX_SEED)
		{
			LOG_ERROR("Seed is disallowed, setting a random seed instead");
			m_Seed = GetRand<unsigned long long>(1000000000, MAX_SEED);
		}

		m_LerpedSeed = Lerp(m_Seed);

		LOG_INFO("World seed: " << m_Seed);
		LOG_INFO("World seed after interpolation: " << m_LerpedSeed);
	}

	void TerrainGenerator::Generate()
	{
		// For chunk counts of 2^4 and up
		int start = 0 - CHUNKS / 8;
		int end = 0 + CHUNKS / 8;

		if (CheckIfGenerated())
		{
			LOG_INFO("Folder with chunks already found, mapping existing chunks...");
			ChunkManager::MapChunks();
			return;
		}

		LOG_INFO("No chunks found, generating new world...")
		std::filesystem::create_directory(WRITE_PATH);

		for (int z = start; z < end; z++)
		{
			for (int x = start; x < end; x++)
			{
				auto chunk = new Chunk();

				glm::vec3 chunkPos(x * CHUNK_X, 0, z * CHUNK_Z);
				chunk->SetPosition(chunkPos.x, chunkPos.y, chunkPos.z);

				Noisify(*chunk);

				ChunkManager::WriteToFile(chunkPos, *chunk);
			}
		}
	}

	bool TerrainGenerator::CheckIfGenerated()
	{
		if (!std::filesystem::is_directory(WRITE_PATH))
			return false;

		size_t fileCount = std::distance(std::filesystem::directory_iterator(WRITE_PATH), std::filesystem::directory_iterator());

		if (fileCount == 0)
			return false;

		return true;
	}

	void TerrainGenerator::Noisify(Chunk& chunk)
	{
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


	double TerrainGenerator::Lerp(unsigned long long val)
	{
		double x = ((double)m_Seed - MIN_SEED) / (MAX_SEED - MIN_SEED);
		return -255.0 + x * (255.0 - -255.0);
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
