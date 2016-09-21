#pragma once
#include "Core_Common.h"
#include "Utility/FileSystem.h"

namespace Vanguard
{
	// A collection of functions for retrieving directory paths of the running instance.
	struct CORE_API Directories
	{
	private:
		Directories(){}

	public:
		static FilePath GetVanguardProjectsDirectory();
		static FilePath GetVanguardRootDirectory();
		static FilePath GetLogDirectory();

		static FilePath GetEngineBinDirectory();
		static FilePath GetEngineModuleDirectory();
		static FilePath GetEngineDirectory();
		static FilePath GetEngineConfigDirectory();
		static FilePath GetEngineResourceDirectory();

		static FilePath GetProjectTempDirectory();
		static FilePath GetProjectDirectory();
		static FilePath GetProjectConfigDirectory();
		static FilePath GetProjectModuleDirectory();
		static FilePath GetProjectResourceDirectory();

		static DynamicArray<FilePath> GetResourceDirectories();
	};
}