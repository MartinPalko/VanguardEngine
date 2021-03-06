#include "Project.h"
#include "Config/ConfigFile.h"
#include "Log.h"

namespace Vanguard
{
	Project::Project(FilePath aProjectDirectory)
	{
		DynamicArray<FilePath> configFiles = FileSystem::Find(aProjectDirectory, "*.cfg", false, true, false);

		DEBUG_ERROR_IF(configFiles.Count() < 1, "Could not find project config file in " + aProjectDirectory.GetFullPathName())

		ConfigFile projectConfigFile = ConfigFile();

		for (size_t i = 0; i < configFiles.Count(); i++)
		{
			projectConfigFile.LoadAdditive(configFiles[i]);
		}

		name = aProjectDirectory.GetFilenameWithoutExtension();
		friendlyName = projectConfigFile.GetValue("Project", "FriendlyName");
		description = projectConfigFile.GetValue("Project", "Description");

		LOG_MESSAGE("Loaded project " + friendlyName + ": " + description,"Projects");
	}
}