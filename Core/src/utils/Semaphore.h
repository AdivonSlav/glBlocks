#pragma once

#include "../CoreAPI.h"

namespace CoreUtils
{
	class Semaphore
	{
	private:
		int m_Count;
		std::mutex m_Mutex;
		std::condition_variable m_ConditionVariable;
	public:
		Semaphore(int count);
		~Semaphore() = default;

		void lock();

		void unlock();

		int GetCount() const { return m_Count; }
	};
}