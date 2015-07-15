#include "Directories.h"
#include "Core.h"

namespace Vanguard
{
	FilePath Directories::GetVanguardProjectsDirectory()
	{
		return GetVanguardRootDirectory().GetRelative("Projects");
	}

	FilePath Directories::GetVanguardRootDirectory()
	{
		return GetEngineBinDirectory().GetParentDirectory();
	}

	FilePath Directories::GetLogDirectory()
	{
		FilePath logDir = GetVanguardRootDirectory().GetRelative("Logs");
		if (!FileSystem::DirectoryExists(logDir))
			FileSystem::CreateDirectory(logDir);
		return logDir;
	}

	// ---------- Engine Directories ----------

	FilePath Directories::GetEngineBinDirectory()
	{
		FilePath engineExecutable = FileSystem::GetHostApplicationPath();
		return engineExecutable.GetParentDirectory();
	}

	FilePath Directories::GetEngineModuleDirectory()
	{
		return GetEngineBinDirectory();
	}

	FilePath Directories::GetEngineDirectory()
	{
		return GetVanguardRootDirectory().GetRelative("Engine");
		//return GetEngineBinDirectory().GetParentDirectory();
	}

	FilePath Directories::GetEngineConfigDirectory()
	{
		FilePath configDir = GetEngineDirectory().GetRelative("Config");
		if (!FileSystem::DirectoryExists(configDir))
			FileSystem::CreateDirectory(configDir);
		return configDir;
	}

	// ---------- Project Directories ----------

	// Gets a directory inside of the users temp directory, useful for temporary files.
	FilePath Directories::GetProjectTempDirectory()
	{
		FilePath engineTempDirectory = FileSystem::GetSystemTempDirectory().GetRelative("Vanguard");

		// Create the directory if it doesn't already exist.
		if (!FileSystem::DirectoryExists(engineTempDirectory))
			FileSystem::CreateDirectory(engineTempDirectory);

		return engineTempDirectory;
	}

	FilePath Directories::GetProjectDirectory()
	{
		return GetVanguardProjectsDirectory().GetRelative(Core::GetInstance()->GetLoadedProject()->GetName());
	}

	FilePath Directories::GetProjectConfigDirectory()
	{
		return GetProjectDirectory().GetRelative("Config");
	}

	FilePath Directories::GetProjectModuleDirectory()
	{
		return GetEngineModuleDirectory();
	}
}