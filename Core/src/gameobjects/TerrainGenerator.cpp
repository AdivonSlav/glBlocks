#include "TerrainGenerator.h"

#include <iostream>
#include <cmath>
#include <random>
#include <glm/gtc/noise.hpp>

#include "../utils/Logger.h"

#define MAX_SEED 9999999999
#define MIN_SEED 1

#define CHUNKS 32
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

		LOG_INFO("World seed: " << m_Seed);
		LOG_INFO("World seed after interpolation: " << Lerp(m_Seed));
	}

	void TerrainGenerator::Generate()
	{
		// For chunk counts of 2^4 and up
		int start = 0 - CHUNKS / 8;
		int end = 0 + CHUNKS / 8;


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

	void TerrainGenerator::Noisify(Chunk& chunk)
	{
		glm::vec3 chunkPos = chunk.GetPos();
		const float increment = 1000.0f;
		const double lerpedSeed = Lerp(m_Seed);

		for (int x = 0; x < CHUNK_X; x++)
		{
			for (int z = 0; z < CHUNK_Z; z++)
			{
				int surfaceY = 100;

				// Getting block coordinates in world space and adding a seed value lerped between -255.0 and 255.0
				float blockX = x + chunkPos.x + lerpedSeed;
				float blockZ = z + chunkPos.z + lerpedSeed;

				// The coordinates are divided by an increment to get smaller steps
				float noise2D = Noise(glm::vec2(blockX / increment, blockZ / increment), 6, 0.9f);
				surfaceY += noise2D * 10.0f;

				for (int y = 0; y < CHUNK_Y; y++)
				{
					if (y == surfaceY - 1)
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
