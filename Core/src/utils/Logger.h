#pragma once

#include <sstream>

#include "../CoreAPI.h"

namespace Utils
{
	enum class CORE_API LogLevel { INFO, WARN, ERROR };

	class CORE_API Logger
	{
	private:
		LogLevel m_Level;
		std::stringstream m_Stream;

		void Log();
		std::string GetLevel();
	public:
		Logger(LogLevel level);
		~Logger();

		std::stringstream& GetStream() { return m_Stream; }
	};

#ifdef BLOCKS_DEBUG
	#define LOG_INFO(Message_) Utils::Logger(Utils::LogLevel::INFO).GetStream() << Message_;
	#define LOG_WARN(Message_) Utils::Logger(Utils::LogLevel::WARN).GetStream() << Message_;
	#define LOG_ERROR(Message_) Utils::Logger(Utils::LogLevel::ERROR).GetStream() << Message_;
#else
	#define LOG_INFO(_)
	#define LOG_WARN(_)
	#define LOG_ERROR(_) 
#endif
}
