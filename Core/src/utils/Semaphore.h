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

		/**
		 * \brief Acquires a lock on the mutex and prevents another thread from acquiring it until the thread count passes a certain value
		 */
		void lock();

		/**
		 * \brief Releases the lock on the mutex, signifying that the thread has finished its execution queue
		 */
		void unlock();

		int GetCount() const { return m_Count; }
	};
}