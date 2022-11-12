#pragma once

#include "ChunkManager.h"
#include "../graphics/Camera.h"

namespace CoreGameObjects
{
	class CORE_API TerrainGenerator
	{
	private:
		static unsigned long long m_Seed;
		static double m_LerpedSeed;

		static Camera* m_Camera;
	private:
	public:
		/**
		 * \brief Constructs a terrain generator and checks the validity of the provided seed
		 * \param seed The world seed
		 */
		TerrainGenerator(unsigned long long seed = 0);
		~TerrainGenerator() = default;

		/**
		 * \brief Initializes a chunk folder if not already present
		 */
		void Init();

		/**
		 * \brief Serializes and deserializes chunks from disc and loads them into memory at a certain distance, then builds them 
		 */
		void LoadChunks();

		/**
		 * \brief Marks chunks based on render status and disposes chunks if needed
		 */
		void SynchronizeChunks();

		/**
		 * \brief Checks whether a world is already generated and logs the number of chunks present
		 * \return True if there are chunks present on disk
		 */
		bool CheckIfGenerated();

		/**
		 * \brief Uploads all chunks to the GPU that should be rendered
		 */
		void PrepareChunks();

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
		 * \brief Procedurally generates the terrain of a chunk
		 * \param chunk The chunk that is to be noisified
		 */
		static void Noisify(Chunk& chunk);

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
		 * \brief Provides the scene camera to the terrain generator
		 * \param camera Camera in the scene
		 */
		void SetCamera(Camera& camera) { m_Camera = &camera;}

		static unsigned long long& GetSeed() { return m_Seed; }
	};
}
