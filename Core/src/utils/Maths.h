#pragma once

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