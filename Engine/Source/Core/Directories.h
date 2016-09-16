#pragma once
#include "Core_Common.h"
#include "Utility/FileSystem.h"

namespace Vanguard
{
	class Directories
	{
	private:
		Directories(){}

	public:
		static CORE_API FilePath GetVanguardProjectsDirectory();
		static CORE_API FilePath GetVanguardRootDirectory();
		static CORE_API FilePath GetLogDirectory();

		static CORE_API FilePath GetEngineBinDirectory();
		static CORE_API FilePath GetEngineModuleDirectory();
		static CORE_API FilePath GetEngineDirectory();
		static CORE_API FilePath GetEngineConfigDirectory();
		static CORE_API FilePath GetEngineResourceDirectory();

		static CORE_API FilePath GetProjectTempDirectory();
		static CORE_API FilePath GetProjectDirectory();
		static CORE_API FilePath GetProjectConfigDirectory();
		static CORE_API FilePath GetProjectModuleDirectory();
		static CORE_API FilePath GetProjectResourceDirectory();

		static CORE_API DynamicArray<FilePath> GetResourceDirectories();
	};
}