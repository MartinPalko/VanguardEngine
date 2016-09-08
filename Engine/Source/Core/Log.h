#pragma once

#include "Core_Common.h"
#include "Foundation.h"

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

		String GetMessage() { return message; }
		String GetCategory() { return channel; }
		LogEntryErrorLevel GetWarningLevel() { return errorLevel; }
		DateAndTime GetTimestamp(){ return timestamp; }

		String GetFormattedLogEntry()
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

	public:
		static void Initialize();

		static void Write(const String& aMessage, const LogEntryErrorLevel& aErrorLevel, const String& aChannel = "");

		static inline void Message(const String& aMessage, const String& aChannel = "") { Write(aMessage, LogEntryErrorLevel::Message, aChannel); }
		static inline void Warning(const String& aMessage, const String& aChannel = "") { Write(aMessage, LogEntryErrorLevel::Warning, aChannel); }
		static inline void Error(const String& aMessage, const String& aChannel = "") { Write(aMessage, LogEntryErrorLevel::Error, aChannel); }
		static inline void Exception(const String& aMessage, const String& aChannel = "") { Write(aMessage, LogEntryErrorLevel::Exception, aChannel); }

		static void Flush();
	};
}

#if VANGUARD_DEBUG
#define DEBUG_LOG(message) {Vanguard::Log::Message(message, "Debug");}
#define DEBUG_WARN(message) {Vanguard::Log::Warning(message, "Debug");}
#define DEBUG_ERROR(message) {Vanguard::Log::Error(message, "Debug");}
#define DEBUG_EXCEPTION(message) {Vanguard::Log::Exception(message, "Debug");}

#define DEBUG_LOG_IF(condition, message) if (condition) {Vanguard::Log::Message(message, "Debug");}
#define DEBUG_WARN_IF(condition, message) if (condition) {Vanguard::Log::Warning(message, "Debug");}
#define DEBUG_ERROR_IF(condition, message) if (condition) {Vanguard::Log::Error(message, "Debug");}
#define DEBUG_EXCEPTION_IF(condition, message) if (condition) {Vanguard::Log::Exception(message, "Debug");}
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

