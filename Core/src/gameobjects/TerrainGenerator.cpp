
#include "PCH.h"

#include "TerrainGenerator.h"
#include "../utils/Dashboard.h"
#include "ChunkManager.h"
#include "../graphics/VertexArrayManager.h"

#define MAX_SEED 9999999999
#define MIN_SEED 1

#define LOAD_DISTANCE 192.0f // Chunks whose distance length is less than this to the camera are loaded into memory
#define RENDER_DISTANCE 160.0f // Chunks whose distance length is less than this to the camera are uploaded to the GPU and rendered

#define CHUNK_LOAD_COUNT 8 // Max number of chunks that can be loaded into memory in a single frame
#define ASYNC_CHUNK_BUILD_COUNT 30 // Max number of chunks that can be forwarded to another thread for building
#define CHUNK_UPLOAD_WAIT_TIME 0.02 // The time to wait before uploading a new chunk to the GPU (in seconds)

namespace CoreGameObjects
{
#define CHUNK_UPLOAD_COUNT 2 // Number of chunks to upload per upload interval
	unsigned long long TerrainGenerator::m_Seed = 0;
	double TerrainGenerator::m_LerpedSeed = 0.0;
	double TerrainGenerator::m_LastUploadTime = 0.0;
	Camera* TerrainGenerator::m_Camera = nullptr;
	Semaphore TerrainGenerator::m_Semaphore(2);

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
		
		int camChunkX = static_cast<int>(std::floor(camPos.x / CHUNK_X));
		int camChunkZ = static_cast<int>(std::floor(camPos.z / CHUNK_Z));
		
		int radius = static_cast<int>(std::ceil(LOAD_DISTANCE / CHUNK_X)) + 1;
		
		struct ChunkCandidate
		{
			glm::vec3 position;
			float distance;
			int ring;
		};
		
		std::vector<ChunkCandidate> candidates;
		
		for (int dx = -radius; dx <= radius; dx++)
		{
			for (int dz = -radius; dz <= radius; dz++)
			{
				glm::vec3 chunkPos((camChunkX + dx) * CHUNK_X, 0.0f, (camChunkZ + dz) * CHUNK_Z);
				
				if (ChunkManager::IsLoaded(chunkPos))
					continue;
				
				glm::vec3 chunkCenter(chunkPos.x + CHUNK_X / 2.0f, 0.0f, chunkPos.z + CHUNK_Z / 2.0f);
				glm::vec2 distanceVector = camPos.xz() - glm::vec2(chunkCenter.x, chunkCenter.z);
				float distance = glm::length(distanceVector);
				
				if (distance <= LOAD_DISTANCE)
				{
					int ring = std::max(std::abs(dx), std::abs(dz));
					candidates.push_back({chunkPos, distance, ring});
				}
			}
		}
		
		if (!candidates.empty())
		{
			std::sort(candidates.begin(), candidates.end(), [](const ChunkCandidate& a, const ChunkCandidate& b) {
				if (a.ring != b.ring)
					return a.ring < b.ring;
				return a.distance < b.distance;
			});
			
			auto seed = static_cast<unsigned int>(m_Seed ^ static_cast<unsigned long long>(camChunkX) ^ (static_cast<unsigned long long>(camChunkZ) << 32));
			std::mt19937 rng(seed);
			
			int currentRing = 0;
			auto ringBegin = candidates.begin();
			
			for (auto it = candidates.begin(); it != candidates.end(); ++it)
			{
				if (it->ring != currentRing)
				{
					auto ringEnd = it;
					std::shuffle(ringBegin, ringEnd, rng);
					ringBegin = it;
					currentRing = it->ring;
				}
			}
			if (ringBegin != candidates.end())
				std::shuffle(ringBegin, candidates.end(), rng);
			
			int chunksLoaded = 0;
			for (const auto& candidate : candidates)
			{
				if (chunksLoaded >= CHUNK_LOAD_COUNT)
					break;
				
				std::shared_ptr<Chunk> chunk = std::make_shared<Chunk>(candidate.position);
				
				if (ChunkManager::IsSerialized(candidate.position))
				{
					ChunkManager::Deserialize(candidate.position, chunk.get(), GetSeed());
					chunk->SetSerialized(true);
				}
				else
				{
					Noisify(*chunk);
					ChunkManager::Serialize(candidate.position, chunk.get(), GetSeed());
				}
				
				ChunkManager::LoadChunk(chunk);
				chunksLoaded++;
				LOG_INFO("Loaded chunk -> " << ChunkManager::GetLoadedChunks().size());
			}
		}

	std::vector<std::shared_ptr<Chunk>> chunksToBuild;
	std::vector<std::shared_ptr<Chunk>> chunksToRebuild;

	for (auto& loadedChunk : ChunkManager::GetLoadedChunks())
	{
		if (ChunkManager::IsPositionQueuedForBuild(loadedChunk->GetPos()))
			continue;

		if (!loadedChunk->IsBuilt())
			chunksToBuild.push_back(loadedChunk);
		else if (loadedChunk->NeedsRebuild())
			chunksToRebuild.push_back(loadedChunk);
	}

	std::sort(chunksToBuild.begin(), chunksToBuild.end(), [this](const std::shared_ptr<Chunk>& a, const std::shared_ptr<Chunk>& b) {
		auto camPos = m_Camera->GetPosition();
		glm::vec3 centerA(a->GetPos().x + CHUNK_X / 2.0f, 0.0f, a->GetPos().z + CHUNK_Z / 2.0f);
		glm::vec3 centerB(b->GetPos().x + CHUNK_X / 2.0f, 0.0f, b->GetPos().z + CHUNK_Z / 2.0f);
		glm::vec2 distA = camPos.xz() - glm::vec2(centerA.x, centerA.z);
		glm::vec2 distB = camPos.xz() - glm::vec2(centerB.x, centerB.z);
		return glm::length(distA) < glm::length(distB);
	});

	std::sort(chunksToRebuild.begin(), chunksToRebuild.end(), [this](const std::shared_ptr<Chunk>& a, const std::shared_ptr<Chunk>& b) {
		auto camPos = m_Camera->GetPosition();
		glm::vec3 centerA(a->GetPos().x + CHUNK_X / 2.0f, 0.0f, a->GetPos().z + CHUNK_Z / 2.0f);
		glm::vec3 centerB(b->GetPos().x + CHUNK_X / 2.0f, 0.0f, b->GetPos().z + CHUNK_Z / 2.0f);
		glm::vec2 distA = camPos.xz() - glm::vec2(centerA.x, centerA.z);
		glm::vec2 distB = camPos.xz() - glm::vec2(centerB.x, centerB.z);
		return glm::length(distA) < glm::length(distB);
	});

	std::vector<std::shared_ptr<Chunk>> chunksToQueue;
	
	for (const auto& chunk : chunksToBuild)
	{
		if (chunksToQueue.size() >= ASYNC_CHUNK_BUILD_COUNT)
			break;
		chunksToQueue.push_back(chunk);
	}

	for (const auto& chunk : chunksToRebuild)
	{
		if (chunksToQueue.size() >= ASYNC_CHUNK_BUILD_COUNT)
			break;
		chunk->SetNeedsRebuild(false);
		chunksToQueue.push_back(chunk);
	}

	for (const auto& chunk : chunksToQueue)
	{
		bool isRebuild = (std::find(chunksToRebuild.begin(), chunksToRebuild.end(), chunk) != chunksToRebuild.end());
		ChunkManager::QueueForBuild(chunk, isRebuild, m_Semaphore);
	}
	}

	void TerrainGenerator::SynchronizeChunks()
	{
		while (!ChunkManager::GetQueuedForBuild().empty() && ChunkManager::IsBuildQueueReady())
		{
			auto builtChunks = ChunkManager::GetQueuedForBuild().front().get();

			for (auto& chunk : builtChunks)
				ChunkManager::MarkPositionAsBuilt(chunk->GetPos());

			ChunkManager::GetQueuedForBuild().pop_front();

			LOG_INFO("Built " << builtChunks.size() << " chunks");
		}

		auto& camPos = m_Camera->GetPosition();

		for (auto loadedIt = ChunkManager::GetLoadedChunks().begin(); loadedIt != ChunkManager::GetLoadedChunks().end(); )
		{
			auto chunkPos = loadedIt->get()->GetPos();
			glm::vec3 chunkCenter(chunkPos.x + CHUNK_X / 2.0f, 0.0f, chunkPos.z + CHUNK_Z / 2.0f);
			glm::vec2 distanceVector = camPos.xz() - glm::vec2(chunkCenter.x, chunkCenter.z);
			auto distance = glm::length(distanceVector);

			if (distance <= RENDER_DISTANCE && loadedIt->get()->IsBuilt() && !loadedIt->get()->ShouldRender())
			{
				loadedIt->get()->SetShouldRender(true);
				LOG_INFO("Marked chunk for rendering");
			}
			else if (distance > RENDER_DISTANCE && loadedIt->get()->IsBuilt() && loadedIt->get()->ShouldRender())
			{
				loadedIt->get()->SetShouldRender(false);
				LOG_INFO("Marked chunk to not render");

				if (loadedIt->get()->GetVAO() != nullptr && !ChunkManager::IsPositionQueuedForBuild(chunkPos))
				{
					VertexArrayManager::CacheVAO(loadedIt->get()->GetVAO());
					loadedIt->get()->GetVAO() = nullptr;
					loadedIt->get()->SetUploaded(false);
				}
			}

			if (distance > LOAD_DISTANCE
				&& loadedIt->get()->IsBuilt()
				&& !ChunkManager::IsPositionQueuedForBuild(chunkPos))
			{
				ChunkManager::SynchronizeObscured(loadedIt->get());
				ChunkManager::RemoveChunkPosition(chunkPos);
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

	void TerrainGenerator::PrepareChunks(double deltaTime)
	{
		m_LastUploadTime += deltaTime;

		if (m_LastUploadTime >= CHUNK_UPLOAD_WAIT_TIME)
		{
			m_LastUploadTime = 0.0;

			auto camPos = m_Camera->GetPosition();
			int uploadsRemaining = CHUNK_UPLOAD_COUNT;

			while (uploadsRemaining > 0)
			{
				std::shared_ptr<Chunk> nearestChunk = nullptr;
				float nearestDistance = std::numeric_limits<float>::max();

				for (auto& chunk : ChunkManager::GetLoadedChunks())
				{
					if (chunk->IsBuilt() && chunk->ShouldRender() && !ChunkManager::IsPositionQueuedForBuild(chunk->GetPos()) && (!chunk->IsUploaded() || chunk->NeedsGPUUpdate()))
					{
					glm::vec3 chunkCenter(chunk->GetPos().x + CHUNK_X / 2.0f, 0.0f, chunk->GetPos().z + CHUNK_Z / 2.0f);
					glm::vec2 distanceVector = camPos.xz() - glm::vec2(chunkCenter.x, chunkCenter.z);
					float distance = glm::length(distanceVector);

						if (distance < nearestDistance)
						{
							nearestDistance = distance;
							nearestChunk = chunk;
						}
					}
				}

				if (!nearestChunk)
					break;

				if (!nearestChunk->IsUploaded())
				{
					nearestChunk->UploadToGPU();
					LOG_INFO("Uploaded chunk to GPU");
				}
				else if (nearestChunk->NeedsGPUUpdate())
				{
					nearestChunk->UpdateGPUData();
					nearestChunk->SetNeedsGPUUpdate(false);
					LOG_INFO("Updated chunk GPU data");
				}
				uploadsRemaining--;
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
				int16_t surfaceY = 32;
				int16_t stoneY = 28;

				// Getting block coordinates in world space and adding a seed value lerped between -255.0 and 255.0
				float blockX = x + chunkPos.x + m_LerpedSeed;
				float blockZ = z + chunkPos.z + m_LerpedSeed;

				// The coordinates are divided by an increment to get smaller steps
				float height = Noise(glm::vec2(blockX / increment, blockZ / increment), 7, 0.9f);
				float stoneHeight = Noise(glm::vec2(blockX / increment, blockZ / increment), 7, 1.0f);
				surfaceY += height * 10.0f;
				stoneY += stoneHeight * 12.0f;

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
		if (CoreUtils::Dashboard::ShouldGenerateWorld())
		{
			for (const auto& entry : std::filesystem::directory_iterator(WRITE_PATH))
			{
				std::filesystem::remove_all(entry);
			}
		}
	}
}
