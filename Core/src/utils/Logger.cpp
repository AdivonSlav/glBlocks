#include "Logger.h"

#include <iostream>

namespace Utils
{

	Logger::Logger(LogLevel level)
		: m_Level(level)
	{

	}

	Logger::~Logger()
	{
		Log();
	}

	void Logger::Log()
	{
		std::cout << "glBlocksCore " << GetLevel() << m_Stream.str() << std::endl;
	}

	std::string Logger::GetLevel()
	{
		switch (m_Level)
		{
		case LogLevel::INFO:
			return "INFO: ";
		case LogLevel::WARN:
			return "WARN: ";
		case LogLevel::ERROR:
			return "ERROR: ";
		}

		return "UNKNOWN: ";
	}
}
