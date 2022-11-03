#pragma once

#include "../CoreAPI.h"

namespace CoreUtils
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

		/**
		 * \brief Gets the current date/time
		 * \param onlyDate Whether to format as only date
		 * \return A string containing the current date/time
		 */
		std::string GetCurrentTime(bool onlyDate = false);

		/**
		 * \brief Writes the log to a file
		 * \param logMessage The log message to log
		 */
		void LogToFile(const std::string& logMessage);

		std::stringstream& GetStream() { return m_Stream; }
	};


#define LOG_INFO(Message_) CoreUtils::Logger(CoreUtils::LogLevel::INFO).GetStream() << Message_;
#define LOG_WARN(Message_) CoreUtils::Logger(CoreUtils::LogLevel::WARN).GetStream() << Message_;
#define LOG_ERROR(Message_) CoreUtils::Logger(CoreUtils::LogLevel::ERROR).GetStream() << Message_;
}
