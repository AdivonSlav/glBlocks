
#include <iostream>
#include <cmath>
#include <random>
#include <filesystem>
#include <iterator>
#include <mutex>

#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>

#include "TerrainGenerator.h"
#include "../utils/Logger.h"
#include "../utils/Maths.h"
#include "ChunkManager.h"

#define MAX_SEED 9999999999
#define MIN_SEED 1

std::mutex mtx;

namespace CoreGameObjects
{
	unsigned long long TerrainGenerator::m_Seed = 0;
	double TerrainGenerator::m_LerpedSeed = 0.0;
	std::atomic<bool> TerrainGenerator::m_MappingChunk = false;
	std::atomic<bool> TerrainGenerator::m_PreparingChunk = false;
	std::atomic<bool> TerrainGenerator::m_LoadingChunk = false;
	std::atomic<bool> TerrainGenerator::m_Terminate = false;
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

	TerrainGenerator::~TerrainGenerator()
	{
		workers[0].join();
		workers[1].join();
	}

	void TerrainGenerator::Init()
	{
		ChunkManager::GetLoadedChunks().reserve(81);

		if (!CheckIfGenerated())
		{
			LOG_INFO("Creating new chunk folder")
			std::filesystem::create_directory(WRITE_PATH);
		}

		workers[1] = std::thread(MapChunks);
		workers[0] = std::thread(PrepareChunks);
	}

	void TerrainGenerator::LoadChunks()
	{
		if (!m_PreparingChunk)
		{
			m_LoadingChunk = true;

			// Chunks that are marked to be disposed by the other thread are removed from the vector before anything else
			DisposeChunks();

			// Checks whether any chunks are no longer prepared by the other thread and unloads them if necessary
			SynchronizeChunks();

			//All chunks prepared by the other thread are loaded for rendering
			for (auto preparedIt = ChunkManager::GetPreparedChunks()->begin(); preparedIt != ChunkManager::GetPreparedChunks()->end(); ++preparedIt)
			{
				if (!ChunkManager::IsLoaded(preparedIt->first) && preparedIt->second->ShouldLoad())
				{
					preparedIt->second->Build();
					ChunkManager::LoadChunk(preparedIt->second);
					LOG_INFO("Loaded from prepared -> " << ChunkManager::GetLoadedChunks().size());
				}
			}

			m_LoadingChunk = false;
		}
	}

	void TerrainGenerator::DisposeChunks()
	{
		for (auto preparedIt = ChunkManager::GetPreparedChunks()->begin(); preparedIt != ChunkManager::GetPreparedChunks()->end(); )
		{
			if (preparedIt->second->ShouldDispose())
			{
				preparedIt = ChunkManager::GetPreparedChunks()->erase(preparedIt);
				LOG_INFO("Disposed chunk -> " << ChunkManager::GetPreparedChunks()->size());
			}
			else
				++preparedIt;
		}
	}

	void TerrainGenerator::SynchronizeChunks()
	{
		for (auto loadedIt = ChunkManager::GetLoadedChunks().begin(); loadedIt != ChunkManager::GetLoadedChunks().end(); )
		{
			if (!loadedIt->get()->ShouldLoad())
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
			return false;

		size_t fileCount = std::distance(std::filesystem::directory_iterator(WRITE_PATH), std::filesystem::directory_iterator());

		LOG_INFO("Found chunk folder with " << fileCount << " chunks");

		return true;
	}

	void TerrainGenerator::PrepareChunks()
	{
		while(!m_Terminate)
		{
			const glm::vec3 cameraPos = m_Camera->GetPosition();

			float prepareDistance = 81.0f;
			float renderDistance = 64.0f;

			if (!m_MappingChunk)
			{
				for (auto mappedIt = ChunkManager::GetMappedChunks().begin(); mappedIt != ChunkManager::GetMappedChunks().end(); ++mappedIt)
				{
					m_PreparingChunk = true;
					glm::vec2 distanceVector = cameraPos.xz - glm::vec2(mappedIt->first.x + CHUNK_X, mappedIt->first.z + CHUNK_Z);
					auto distance = glm::length(distanceVector);

					if (distance <= prepareDistance && !ChunkManager::IsPrepared(mappedIt->first) && !m_LoadingChunk)
					{
						auto chunk = ChunkManager::ReadFromFile(mappedIt->first, m_Seed);
						chunk->SetPosition(mappedIt->first);
						
						if (distance <= renderDistance && !chunk->ShouldLoad())
						{
							chunk->SetShouldLoad(true);
							LOG_INFO("Marked chunk for loading")
						}

						ChunkManager::PrepareChunk(chunk);
						LOG_INFO("Prepared chunk -> " << ChunkManager::GetPreparedChunks()->size());
					}

					if (distance <= renderDistance && ChunkManager::IsPrepared(mappedIt->first) && !m_LoadingChunk)
					{
						auto chunk = ChunkManager::GetPreparedChunk(mappedIt->first);
						if (!chunk->ShouldLoad())
						{
							chunk->SetShouldLoad(true);
							LOG_INFO("Marked chunk for loading")
						}
					}

					m_PreparingChunk = false;
				}
			}

			m_PreparingChunk = true;
			for (auto& preparedChunk : *ChunkManager::GetPreparedChunks())
			{
				glm::vec2 distanceVector = cameraPos.xz - glm::vec2(preparedChunk.first.x + CHUNK_X, preparedChunk.first.z + CHUNK_Z);
				auto distance = glm::length(distanceVector);

				if (distance > prepareDistance && !preparedChunk.second->ShouldDispose())
				{
					preparedChunk.second->SetShouldDispose(true);
					LOG_INFO("Marked chunk for disposal");
				}

				if (distance > renderDistance && preparedChunk.second->ShouldLoad())
				{
					preparedChunk.second->SetShouldLoad(false);
					LOG_INFO("Marked chunk for unloading");
				}

			}
			m_PreparingChunk = false;

		}
	}

	void TerrainGenerator::MapChunks()
	{
		while (!m_Terminate)
		{
			const glm::vec3 cameraPos = m_Camera->GetPosition();
 			glm::vec2 cameraPosInChunks = {cameraPos.x / 16, cameraPos.z / 16};
			                                                           
			glm::ivec2 xBoundaries =
			{                                          
				cameraPosInChunks.x - (8 / 2),             
				cameraPosInChunks.x + (8 / 2)
			};

			glm::ivec2 zBoundaries =
			{
				cameraPosInChunks.y - (8 / 2),
				cameraPosInChunks.y + (8 / 2)
			};

			for (int z = zBoundaries.x; z <= zBoundaries.y; z++)
			{
				for (int x = xBoundaries.x; x <= xBoundaries.y; x++)
				{
					glm::vec3 chunkPos = {x * CHUNK_X, 0.0f, z * CHUNK_Z};

					if (!ChunkManager::IsMapped(chunkPos))
					{
						m_MappingChunk = true;
						if (ChunkManager::IsWritten(chunkPos))
						{
							ChunkManager::MapChunk(chunkPos);
							LOG_INFO("Mapping chunk from disk -> " << ChunkManager::GetMappedChunks().size());
						}
						else
						{
							Chunk newChunk(chunkPos);
							Noisify(newChunk);
							ChunkManager::WriteToFile(chunkPos, &newChunk, m_Seed);
							ChunkManager::MapChunk(chunkPos);
							LOG_INFO("Writing chunk to disk -> " << ChunkManager::GetMappedChunks().size());
						}
						m_MappingChunk = false;
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

	void TerrainGenerator::Cleanup()
	{
		m_Terminate = true;
		m_Camera = nullptr;
	}
}
