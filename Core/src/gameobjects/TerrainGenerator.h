#pragma once

#include "ChunkManager.h"

namespace CoreGameObjects
{
	class CORE_API TerrainGenerator
	{
	private:
		unsigned long long m_Seed;
		double m_LerpedSeed;
	private:
		double Lerp(unsigned long long val);

		/**
		 * \brief Procedurally generates the terrain of a chunk
		 * \param chunk The chunk that is to be noisified
		 */
		void Noisify(Chunk& chunk);
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
		 * \brief Loads or unloads any chunks as necessary based on camera positions
		 * \param camera The camera object in the scene
		 */
		void CheckChunkStatus(const Camera& camera);

		/**
		 * \brief Generates a new chunk if necessary and maps it
		 * \param camera The camera object in the scene
		 */
		void Generate(const Camera& camera);

		/**
		 * \brief Checks whether a world is already generated. If generated, Generate() is skipped
		 * \return True if there are chunks present on disk
		 */
		bool CheckIfGenerated();

		static void ChunkWorker();

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
	};
}
