#include "Log.h"
#include "Platform.h"
#include "Config.h"

namespace Vanguard
{
	Int32ConfigVar Log::maxLogFiles = Int32ConfigVar("Core", "Log", "MaxLogFiles", 10);
	Int32ConfigVar Log::maxEntriesBetweenFlushes = Int32ConfigVar("Core", "Log", "MaxEntriesBetweenFlushes", 5);

	bool Log::initialized = false;

	DynamicArray<LogEntry> Log::unflushedEntries;
	Mutex Log::logMutex;
	FilePath Log::logFile;

	void Log::Initialize()
	{
		if (initialized == true)
			return;

		// Clean up old log files.
		DynamicArray<FilePath> logFiles = FileSystem::Find(FileSystem::GetLogDirectory(),"*.log",false);
		Message("Max Log Files: " + String::FromInt32(maxLogFiles));

		while (logFiles.Count() >= maxLogFiles)
		{
			size_t oldestFile = 0;
			Time oldestCreationTime = FileSystem::GetTimeCreated(logFiles[oldestFile]);

			for (size_t i = 1; i < logFiles.Count(); i++)
			{
				Time currentCreationTime = FileSystem::GetTimeCreated(logFiles[oldestFile]);
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
		String fileName = Time::GetCurrentTime().ToString(true, true, false, false);
		fileName = fileName.Replace(' ', '-');
		fileName = fileName.Replace(':', '.');

		logFile = FileSystem::MakeUniqueFileName(FileSystem::GetLogDirectory().GetRelative(fileName + ".log"));
		FileSystem::CreateFile(logFile);

		Flush();

		initialized = true;
	}

	void Log::Write(const String& aMessage, const LogEntryErrorLevel& aErrorLevel, const String& aCategoty)
	{
		LogEntry newEnty(aMessage, aErrorLevel, aCategoty);

		logMutex.Lock();

		std::cout << newEnty.GetFormattedLogEntry() << "\n";
		unflushedEntries.PushBack(newEnty);

		if (aErrorLevel >= LogEntryErrorLevel::Error || unflushedEntries.Count() >= maxEntriesBetweenFlushes)
			Flush();

		if (aErrorLevel == LogEntryErrorLevel::Exception)
			throw Vanguard::Exception(newEnty.GetMessage().GetCharPointer());

		logMutex.Unlock();
	}

	void Log::Flush()
	{
		static bool flushingLog(false);

		if (!flushingLog)
		{
			flushingLog = true;
			
			String textToWrite = "";

			for (size_t i = 0; i < unflushedEntries.Count(); i++)
			{
				textToWrite += unflushedEntries[i].GetFormattedLogEntry() + "\n";
			}
			unflushedEntries.Clear();

			if (!FileSystem::AppendToFile(logFile, textToWrite))
				Exception("Error writing to log file");

			flushingLog = false;
		}
	}
}