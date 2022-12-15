#include "PCH.h"

#include "Semaphore.h"

namespace CoreUtils
{
	Semaphore::Semaphore(int count)
		: m_Count(count)
	{

	}

	void Semaphore::lock()
	{
		std::unique_lock<std::mutex> lock(m_Mutex);

		m_ConditionVariable.wait(lock, [this]
		{
			return m_Count != 0;
		});

		m_Count--;
	}

	void Semaphore::unlock()
	{
		std::unique_lock<std::mutex> lock(m_Mutex);
		m_Count++;
		m_ConditionVariable.notify_one();
	}
}
