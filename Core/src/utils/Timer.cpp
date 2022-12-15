#include "PCH.h"

#define NANOSECONDS 1000000000.0

using namespace std::chrono;

namespace CoreUtils
{
	Timer::Timer()
		: m_ElapsedTime(0.0), m_DeltaTime(0.0)
	{

	}

	void Timer::Start()
	{
		m_LastTime = steady_clock::now();
	}

	void Timer::RecordLapse()
	{
		m_CurrentTime = steady_clock::now();
		m_DeltaTime = duration_cast<nanoseconds>(m_CurrentTime - m_LastTime).count() / NANOSECONDS;
		m_ElapsedTime += m_DeltaTime;
		m_LastTime = m_CurrentTime;
	}
}
