#include "PCH.h"

namespace CoreUtils
{

	Logger::Logger(LogLevel level)
		: m_Level(level)
	{
		std::ios::sync_with_stdio(false);
	}

	Logger::~Logger()
	{
		Log();
	}

	std::string Logger::GetCurrentTime(bool onlyDate)
	{
		auto now = std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::system_clock::now());

		std::string formattedTime = std::format("{:%Y-%m-%d %T}", now);

		if (onlyDate)
			formattedTime = std::format("{:%Y-%m-%d}", now);

		return formattedTime;
	}

	void Logger::LogToFile(const std::string& logMessage)
	{
		std::ofstream stream(GetCurrentTime(true) + ".txt", std::fstream::out | std::fstream::app);

		if (stream.fail())
		{
			std::cerr << strerror(errno);
			std::cout << "\n";
		}

		if (stream.is_open())
		{
			stream << logMessage << "\n";
		}

		stream.close();
	}

	void Logger::Log()
	{
		std::stringstream logMessage;
		logMessage << GetCurrentTime() << " -> " << GetLevel() << m_Stream.str() << std::endl;
#ifdef BLOCKS_DEBUG
		std::cout << logMessage.str();
#endif

		LogToFile(logMessage.str());
	}

	std::string Logger::GetLevel()
	{
		switch (m_Level)
		{
		case LogLevel::INFO:
			//system("Color 02");
			return "INFO: ";
		case LogLevel::WARN:
			//system("Color 06");
			return "WARN: ";
		case LogLevel::ERROR:
			//system("Color 04");
			return "ERROR: ";
		}

		return "UNKNOWN: ";
	}
}
