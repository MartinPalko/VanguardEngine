#include "ConfigTable.h"
#include "ConfigVar.h"

namespace Vanguard
{
	std::map <String, std::map <String, std::map <String, String>>> ConfigTable::configValues = std::map <String, std::map <String, std::map <String, String>>>();

	void ConfigTable::OnConfigVarCreated(const IConfigVar& aNewVar, const String& aConfigDefault)
	{
		// Set to default if it doesn't exist yet.
		if (configValues.find(aNewVar.file) == configValues.end() ||
			configValues[aNewVar.file].find(aNewVar.section) == configValues[aNewVar.file].end() ||
			configValues[aNewVar.file][aNewVar.section].find(aNewVar.name) == configValues[aNewVar.file][aNewVar.section].end())
		{
			configValues[aNewVar.file][aNewVar.section][aNewVar.name] = aConfigDefault;
		}
	}

	String ConfigTable::GetConfigValueText(const IConfigVar& aConfigVar)
	{
		return configValues[aConfigVar.file][aConfigVar.section][aConfigVar.name];
	}

	bool ConfigTable::SaveConfigToDisk()
	{
		for (auto const& file : configValues)
		{
			String fileContent = String();

			for (auto const& section : configValues[file.first])
			{
				fileContent += "\n";
				fileContent += "[";
				fileContent += section.first.Trim();
				fileContent += "]";
				fileContent += "\n";

				for (auto const& name : configValues[file.first][section.first])
				{
					fileContent += name.first.Trim();
					fileContent += "=";
					fileContent += name.second.Trim();
					fileContent += "\n";
				}
			}

			FilePath configFilePath = FileSystem::GetEngineConfigDirectory().GetRelative(file.first + ".cfg");
			FileSystem::WriteToFile(configFilePath, fileContent);
		}
		return true;
	}

	bool ConfigTable::LoadConfigFromDisk()
	{
		List<FilePath> ConfigFiles = FileSystem::Find(FileSystem::GetEngineConfigDirectory(), "*.cfg");

		for (uint32 f = 0; f < ConfigFiles.Size(); f++)
		{
			FilePath configFile = ConfigFiles[f];
			String fileName = configFile.GetFilenameWithoutExtension();

			if (!FileSystem::FileExists(configFile))
				return false;

			List<String> lines = FileSystem::ReadFileLinesAsText(configFile);

			String CurrentSection = "";

			for (uint32 i = 0; i < lines.Size(); i++)
			{
				// Disregard full line comments
				if (lines[i].BeginsWithAny(";#"))
					continue;

				// Remove inline comments
				uint32 commentStartIndex = lines[i].FirstIndexOfAny(";#");
				if (commentStartIndex != -1)
					lines[i] = lines[i].RemoveAfter(commentStartIndex - 1);

				// Section marker
				if (lines[i].BeginsWith('['))
				{
					CurrentSection = lines[i].Trim("[]");
					continue;
				}

				List<String> pair = lines[i].Split('=');
				if (pair.Size() != 2) // Line is invalid, so skip
					continue;

				configValues[fileName][CurrentSection][pair[0]] = pair[1];
			}

		}
		return true;
	}
}
