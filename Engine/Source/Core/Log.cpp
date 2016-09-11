#include <iostream>

#include "Log.h"

#include "Platforms/Platform.h"
#include "Config/Config.h"
#include "Core.h"


namespace Vanguard
{
	Int32ConfigVar Log::maxLogFiles = Int32ConfigVar("Core", "Log", "MaxLogFiles", 10);
	Int32ConfigVar Log::maxEntriesBetweenFlushes = Int32ConfigVar("Core", "Log", "MaxEntriesBetweenFlushes", 5);
	BooleanConfigVar Log::rollingLogFileEnabled = BooleanConfigVar("Core", "Log", "RollingLogfileEnabled", false);

	bool Log::initialized = false;

	FilePath Log::logFile;
	FilePath Log::rollingLogFile;

	// Retreive as a function-static variable so it will always be initialized, even when calling before main (from config var, or reflection system setup etc.)
	DynamicArray<LogEntry>& GetUnflushedEntriesArray()
	{
		static DynamicArray<LogEntry> unflushedEntriesArray = DynamicArray<LogEntry>();
		return unflushedEntriesArray;
	}

	void Log::Initialize()
	{
		if (initialized)
			return;

		String logfilePrefix = Core::GetInstance()->GetLoadedProject()->GetName();

		// Clean up old log files.
		DynamicArray<FilePath> logFiles = FileSystem::Find(Directories::GetLogDirectory(), logfilePrefix + "*.log", false);
		while (logFiles.Count() >= maxLogFiles)
		{
			size_t oldestFile = 0;
			DateAndTime oldestCreationTime = FileSystem::GetTimeCreated(logFiles[oldestFile]);
			for (size_t i = 1; i < logFiles.Count(); i++)
			{
				DateAndTime currentCreationTime = FileSystem::GetTimeCreated(logFiles[oldestFile]);
				if (currentCreationTime < oldestCreationTime)
				{
					oldestFile = i;
					oldestCreationTime = currentCreationTime;
				}
			}
			FileSystem::Delete(logFiles[oldestFile]);
			logFiles.RemoveAt(oldestFile);
		}

		// Create a new log file, named after the current time.
		String fileName = logfilePrefix + "_" + DateAndTime::CurrentTime().ToString(true, true, false, false);
		fileName = fileName.Replace(' ', '-');
		fileName = fileName.Replace(':', '.');

		logFile = Directories::GetLogDirectory();
		logFile = logFile.GetRelative(fileName + ".log");
		logFile = FileSystem::MakeUniqueFileName(logFile);

		rollingLogFile = Directories::GetLogDirectory();
		rollingLogFile = rollingLogFile.GetRelative("rollingLog.log");
		FileSystem::CreateFile(logFile);

		if (rollingLogFileEnabled && !FileSystem::FileExists(rollingLogFile))
			FileSystem::CreateFile(rollingLogFile);

		Flush();

		initialized = true;
	}

	void Log::Write(const String& aMessage, const LogEntryErrorLevel& aErrorLevel, const String& aCategoty)
	{
		static std::mutex logWriteMutex;
		std::lock_guard<std::mutex> lock(logWriteMutex);
		
		const LogEntry newEntry(aMessage, aErrorLevel, aCategoty);
		GetUnflushedEntriesArray().PushBack(newEntry);
		std::cout << newEntry.GetFormattedLogEntry().GetCharPointer() << "\n";

		if (initialized && (aErrorLevel >= LogEntryErrorLevel::Error || GetUnflushedEntriesArray().Count() >= maxEntriesBetweenFlushes))
		{
			Flush();

			if (aErrorLevel == LogEntryErrorLevel::Exception)
			{
				// Allow IO thread to finish writing log file before continuing.
				AsyncIO::JoinIOThread();
			}
		}
	}

	void Log::Flush()
	{
		if (!initialized)
			return;

		static std::atomic<bool> flushingLog(false);

		if (!flushingLog)
		{
			flushingLog = true;

			// Write to log file
			String textToWrite = "";
			for (size_t i = 0; i < GetUnflushedEntriesArray().Count(); i++)
			{
				textToWrite += GetUnflushedEntriesArray()[i].GetFormattedLogEntry() + "\n";
			}

			AsyncIO::AppendToFile(logFile, textToWrite);
			if (rollingLogFileEnabled)
				AsyncIO::AppendToFile(rollingLogFile, textToWrite);

			GetUnflushedEntriesArray().Clear();
			flushingLog = false;
		}
	}
}
