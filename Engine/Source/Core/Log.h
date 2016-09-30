#pragma once

#include "Core_Common.h"
#include "Foundation.h"

//#define VANGUARD_DISABLE_LOGGING

namespace Vanguard
{
	enum class LogEntryErrorLevel : uint8
	{
		Message = 0,
		Warning = 1,
		Error = 2,
		Exception = 3,
		qty = 4
	};

	struct CORE_API LogEntry
	{
		friend class Log;
	private:
		String message;
		String channel;
		LogEntryErrorLevel errorLevel;
		DateAndTime timestamp;
	public:

		// Constructor
		LogEntry(String aMessage, LogEntryErrorLevel aErrorLevel, String aChannel)
			: message(aMessage),
			channel(aChannel),
			errorLevel(aErrorLevel),
			timestamp(DateAndTime::CurrentTime()){}

		String GetMessage() const { return message; }
		String GetCategory() const { return channel; }
		LogEntryErrorLevel GetWarningLevel() const { return errorLevel; }
		DateAndTime GetTimestamp() const { return timestamp; }

		String GetFormattedLogEntry() const
		{
			String messagePrefix = "[" + timestamp.ToString(true, true, true, false) + "] ";

			if (channel != "")
				messagePrefix += "(" + channel + ") ";

			switch (errorLevel)
			{
			case LogEntryErrorLevel::Warning:
				messagePrefix += "Warning: ";
				break;
			case LogEntryErrorLevel::Error:
				messagePrefix += "Error: ";
				break;
			case LogEntryErrorLevel::Exception:
				messagePrefix += "Exception: ";
				break;
			default:
				break;
			}

			return messagePrefix + message;
		}
	};

	class Int32ConfigVar;
	class BooleanConfigVar;

	class CORE_API Log
	{
		friend class Core;
	private:
		static Int32ConfigVar maxLogFiles;
		static Int32ConfigVar maxEntriesBetweenFlushes;
		static BooleanConfigVar rollingLogFileEnabled;

		static bool initialized;

		static FilePath logFile;
		static FilePath rollingLogFile;

		// Called from Core on startup. Creates the log file, named appropriately, and prepares for writing.
		static void Initialize();

	public:
		// Write should generally not be called directly, a set of macros are provided.
		static void Write(const String& aMessage, const LogEntryErrorLevel& aErrorLevel, const String& aChannel);

		// Write all pending log messages to the log file.
		static void Flush();
	};
}

#if ! defined(VANGUARD_DISABLE_LOGGING)
	#define LOG_MESSAGE(message, channel) {Vanguard::Log::Write(message, Vanguard::LogEntryErrorLevel::Message, channel);}
	#define LOG_WARNING(message, channel) {Vanguard::Log::Write(message, Vanguard::LogEntryErrorLevel::Warning, channel);}
	#define LOG_ERROR(message, channel) {Vanguard::Log::Write(message, Vanguard::LogEntryErrorLevel::Error, channel);}
	#define LOG_EXCEPTION(message, channel) {Vanguard::Log::Write(message, Vanguard::LogEntryErrorLevel::Exception, channel); throw Vanguard::Exception(String(message).GetCharPointer());}
#else
	#define LOG_MESSAGE(message, channel)
	#define LOG_WARNING(message, channel)
	#define LOG_ERROR(message, channel)
	#define LOG_EXCEPTION(message, channel) throw Vanguard::Exception(String(message).GetCharPointer());
#endif


#if defined(VANGUARD_DEBUG) && ! defined(VANGUARD_DISABLE_LOGGING)
	#define DEBUG_LOG(message) LOG_MESSAGE(message, "Debug")
	#define DEBUG_WARN(message) LOG_WARNING(message, "Debug")
	#define DEBUG_ERROR(message) LOG_ERROR(message, "Debug")
	#define DEBUG_EXCEPTION(message) LOG_EXCEPTION(message, "Debug")

	#define DEBUG_LOG_IF(condition, message) if (condition) LOG_MESSAGE(message, "Debug")
	#define DEBUG_WARN_IF(condition, message) if (condition) LOG_WARNING(message, "Debug")
	#define DEBUG_ERROR_IF(condition, message) if (condition) LOG_ERROR(message, "Debug")
	#define DEBUG_EXCEPTION_IF(condition, message) if (condition) LOG_EXCEPTION(message, "Debug")
#else
	// Compile out to nothing in non-debug builds.
	#define DEBUG_LOG(message)
	#define DEBUG_WARN(message)
	#define DEBUG_ERROR(message)
	#define DEBUG_EXCEPTION(message)

	#define DEBUG_LOG_IF(condition, message)
	#define DEBUG_WARN_IF(condition, message)
	#define DEBUG_ERROR_IF(condition, message)
	#define DEBUG_EXCEPTION_IF(condition, message)
#endif
