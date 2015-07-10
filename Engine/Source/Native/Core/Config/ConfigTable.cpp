#include "ConfigTable.h"
#include "ConfigVar.h"
#include "Directories.h"

namespace Vanguard
{
	//std::map <String, std::map <String, std::map <String, String> > > ConfigTable::configValues = std::map <String, std::map <String, std::map <String, String> > >();
	Dictionary<String, ConfigFile> ConfigTable::configFiles;

	void ConfigTable::OnConfigVarCreated(const IConfigVar& aNewVar, const String& aConfigDefault)
	{
		// Set to default if it doesn't exist yet.
		if (!configFiles.Contains(aNewVar.file) || !configFiles[aNewVar.file].ContainsValue(aNewVar.section, aNewVar.name))
		{
			configFiles[aNewVar.file].SetValue(aNewVar.section, aNewVar.name,aConfigDefault);
		}
	}

	String ConfigTable::GetConfigValueText(const IConfigVar& aConfigVar)
	{
		return configFiles[aConfigVar.file].GetValue(aConfigVar.section, aConfigVar.name);
	}

	bool ConfigTable::LoadConfigFromDisk()
	{
		DynamicArray<FilePath> ConfigFilePaths = FileSystem::Find(Directories::GetEngineConfigDirectory(), "*.cfg");

		for (uint32 f = 0; f < ConfigFilePaths.Count(); f++)
		{
			FilePath configFile = ConfigFilePaths[f];
			String fileName = configFile.GetFilenameWithoutExtension();
			
			configFiles[fileName] = ConfigFile::Load(ConfigFilePaths[f]);
		}
		return true;
	}
}
