#pragma once

namespace CoreUtils
{
	/**
	 * \brief Interpolates a value between start1 and end1 into a value between start2 and end2
	 * \tparam InterpolationValue Type of value that is being interpolated
	 * \param value Value to be interpolated
	 * \param start1 Range start of the input value
	 * \param end1 Range end of the input value
	 * \param start2 Target range start
	 * \param end2 Target range end
	 * \return The interpolated value between start2 and end2
	 */
	template<typename InterpolationValue>
	float Interpolate(InterpolationValue value, float start1, float end1, float start2, float end2)
	{
		return start2 + (value - start1) / (end1 - start1) * (end2 - start2);
	}

	/**
	 * \brief Randomly generates a number. Is thread safe
	 * \tparam Numeral The type of number to be randomly generated 
	 * \param start Start of the range of numbers that can be randomly generated
	 * \param end End of the range of numbers that can be randomly generated
	 * \return The randomly generated number
	 */
	template <typename Numeral>
	Numeral GetRand(Numeral start, Numeral end)
	{
		thread_local static std::mt19937 generator(std::random_device{}());

		// Switch between integer or real distribution of numbers based on whether the method should return an integer or a float/double
		using distType = typename std::conditional<
			std::is_integral<Numeral>::value, std::uniform_int_distribution<Numeral>, std::uniform_real_distribution<Numeral>
		>::type;

		thread_local static distType distribution;

		return distribution(generator, typename distType::param_type{ start, end });
	}
}