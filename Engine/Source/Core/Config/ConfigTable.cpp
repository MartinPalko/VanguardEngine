#include "ConfigTable.h"
#include "ConfigVar.h"
#include "Directories.h"
#include "Log.h"

namespace Vanguard
{
	Dictionary<String, ConfigFile>& ConfigFiles()
	{
		static Dictionary<String, ConfigFile> configFiles;
		return configFiles;
	}

	void ConfigTable::OnConfigVarCreated(const ConfigVar& aNewVar, const String& aConfigDefault)
	{
		//DEBUG_LOG("Config var created: " + aNewVar.file + "/" + aNewVar.section + "/" + aNewVar.name);

		// Set to default if it doesn't exist yet.
		if (!ConfigFiles().Contains(aNewVar.file) || !ConfigFiles()[aNewVar.file].ContainsValue(aNewVar.section, aNewVar.name))
		{
			ConfigFiles()[aNewVar.file].SetValue(aNewVar.section, aNewVar.name,aConfigDefault);
		}
	}

	String ConfigTable::GetConfigValueText(const ConfigVar& aConfigVar)
	{
		return ConfigFiles()[aConfigVar.file].GetValue(aConfigVar.section, aConfigVar.name);
	}

	void ConfigTable::LoadConfigFromDisk()
	{
		// Engine configs
		DynamicArray<FilePath> engineConfigFilePaths = FileSystem::Find(Directories::GetEngineConfigDirectory(), "*.cfg");

		for (uint32 i = 0; i < engineConfigFilePaths.Count(); i++)
		{
			FilePath configFile = engineConfigFilePaths[i];
			String fileName = configFile.GetFilenameWithoutExtension();
			
			if (ConfigFiles().ContainsKey(fileName))
				ConfigFiles()[fileName].LoadAdditive(engineConfigFilePaths[i]);
			else
				ConfigFiles()[fileName] = ConfigFile::Load(engineConfigFilePaths[i]);
		}

		// Project configs
		DynamicArray<FilePath> projectConfigFilePaths = FileSystem::Find(Directories::GetProjectConfigDirectory(), "*.cfg");

		for (uint32 i = 0; i < projectConfigFilePaths.Count(); i++)
		{
			FilePath configFile = projectConfigFilePaths[i];
			String fileName = configFile.GetFilenameWithoutExtension();

			if (ConfigFiles().ContainsKey(fileName))
				ConfigFiles()[fileName].LoadAdditive(projectConfigFilePaths[i]);
			else
				ConfigFiles()[fileName] = ConfigFile::Load(projectConfigFilePaths[i]);
		}
	}
}
