#pragma once
#include "Utility/FileSystem.h"

namespace Vanguard
{
	class Directories
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