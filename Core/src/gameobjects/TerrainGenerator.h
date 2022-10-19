#pragma once

#include "ChunkManager.h"

namespace CoreGameObjects
{
	class CORE_API TerrainGenerator
	{
	private:
		unsigned long long m_Seed;

		double Lerp(unsigned long long val);
	public:
		TerrainGenerator(unsigned long long seed = 0);
		~TerrainGenerator() = default;

		void Generate();

		void Noisify(Chunk& chunk);

		template<typename Numeral>
		static Numeral GetRand(Numeral start, Numeral end);


		static float Noise(glm::vec2 coordinates, int seed, int octaves, float persistence);
		static float Noise(glm::vec3 coordinates, int seed, int octaves, float persistence);
	};
}
