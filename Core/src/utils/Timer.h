#pragma once

#include "../CoreAPI.h"

namespace CoreUtils
{
	class CORE_API Timer
	{
	private:
		std::chrono::time_point<std::chrono::steady_clock> m_LastTime;
		std::chrono::time_point<std::chrono::steady_clock> m_CurrentTime;

		// Elapsed time since the timer was started (in seconds)
		double m_ElapsedTime;
		// Time delta between last recorded time and the current time (in seconds)
		double m_DeltaTime;

	public:
		Timer();
		~Timer() = default;

		/**
		 * \brief Records the current time before RecordLapse() can be called
		 */
		void Start();

		/**
		 * \brief Advances the elapsed time since Start() was called and sets the time delta between this and the last RecordLapse() call
		 */
		void RecordLapse();

		const double &GetElapsed() const { return m_ElapsedTime; }
		const double &GetDelta() const { return m_DeltaTime; }
	};
}