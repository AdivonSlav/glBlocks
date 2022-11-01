#pragma once

#include <thread>

#include "ChunkManager.h"

namespace CoreGameObjects
{
	class CORE_API TerrainGenerator
	{
	private:
		static unsigned long long m_Seed;
		static double m_LerpedSeed;

		static std::atomic<bool> m_MappingChunk;
		static std::atomic<bool> m_PreparingChunk;
		static std::atomic<bool> m_LoadingChunk;
		static std::atomic<bool> m_Terminate;

		std::thread workers[2];

		static Camera* m_Camera;
	private:
		/**
		 * \brief Procedurally generates the terrain of a chunk
		 * \param chunk The chunk that is to be noisified
		 */
		static void Noisify(Chunk& chunk);
	public:
		/**
		 * \brief Constructs a terrain generator and checks the validity of the provided seed
		 * \param seed The world seed
		 */
		TerrainGenerator(unsigned long long seed = 0);
		~TerrainGenerator();

		/**
		 * \brief Initializes a chunk folder if not already present
		 */
		void Init();

		/**
		 * \brief Loads all chunks that are prepared
		 */
		void LoadChunks();

		/**
		 * \brief Disposes all chunks that are marked to be disposed by the second thread
		 */
		void DisposeChunks();

		/**
		 * \brief Synchronizes the prepared and loaded chunk arrays, thereby unloading chunks that are no longer prepared if necessary
		 */
		void SynchronizeChunks();

		/**
		 * \brief Checks whether a world is already generated and logs the number of chunks present
		 * \return True if there are chunks present on disk
		 */
		bool CheckIfGenerated();

		/**
		 * \brief Constantly updates the prepared chunks map in a worker thread based on load and render distance
		 */
		static void PrepareChunks();

		/**
		 * \brief Constantly maps chunks from disk and writes new chunks if necessary in a worker thread
		 */
		static void MapChunks();

		/**
		 * \brief Randomly generates a number. Is thread safe
		 * \tparam Numeral The type of number to be randomly generated 
		 * \param start Start of the range of numbers that can be randomly generated
		 * \param end End of the range of numbers that can be randomly generated
		 * \return The randomly generated number
		 */
		template<typename Numeral>
		static Numeral GetRand(Numeral start, Numeral end);

		/**
		 * \brief Generates 2D Perlin noise based on the coordinates provided
		 * \param coordinates The coordinates to sample from
		 * \param octaves The number of octaves to pass through (Each successive octave's Perlin value is summed 
		 * \param persistence How much each successive octave contributes to the quality of the noise
		 * \return The sum of the values created by Perlin 
		 */
		static float Noise(const glm::vec2& coordinates, int octaves, float persistence);

		/**
		 * \brief Generates 3D Perlin noise based on the coordinates provided
		 * \param coordinates The coordinates to sample from
		 * \param octaves The number of octaves to pass through (Each successive octave's Perlin value is summed
		 * \param persistence How much each successive octave contributes to the quality of the noise
		 * \return The sum of the values created by Perlin
		 */
		static float Noise(const glm::vec3& coordinates, int octaves, float persistence);

		/**
		 * \brief Sets the terminate flag on every worker thread
		 */
		static void Cleanup();

		/**
		 * \brief Provides the scene camera to the terrain generator
		 * \param camera Camera in the scene
		 */
		void SetCamera(Camera& camera) { m_Camera = &camera;}
	};
}
