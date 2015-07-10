#pragma once
#include "FileSystem.h"

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

		// ---------- Engine Directories ----------

		static FilePath GetEngineBinDirectory();
		static FilePath GetEngineModuleDirectory();
		static FilePath GetEngineDirectory();
		static FilePath GetEngineConfigDirectory();

		// ---------- Project Directories ----------

		static FilePath GetProjectTempDirectory();
		static FilePath GetProjectDirectory();
		static FilePath GetProjectConfigDirectory();
		static FilePath GetProjectModuleDirectory();
	};
}