#pragma once
#include <map>

#include "IntegerDefs.h"
#include "VanguardString.h"
#include "FileSystem.h"
#include "ConfigVar.h"

namespace Vanguard
{
	static const FilePath configFile = FileSystem::GetEngineConfigDirectory() + "Engine.cfg";

	class EngineConfig
	{
		template <class T> class ConfigVar;
		friend class ConfigVar<class T>;

	private:
		static std::map <String, String> configValues;

	public:
		static void OnConfigVarCreated (const String& aConfigName, const String& aConfigDefault)
		{
			if (configValues.find(aConfigName) == configValues.end())
			{
				configValues[aConfigName] = aConfigDefault;
			}
		}

		static String GetConfigValueText (const String& aConfigName)
		{
			return configValues[aConfigName];
		}

	public:

		static bool SaveConfigToDisk()
		{
			String fileContent = String();

			for (auto const& item : configValues)
			{
				fileContent += item.first.Trim();
				fileContent += "=";
				fileContent += item.second.Trim();
				fileContent += "\n";
			}

			return FileSystem::WriteToFile(configFile, fileContent);

		}

		static bool LoadConfigFromDisk()
		{
			if (!FileSystem::FileExists(configFile))
				return false;

			List<String> lines = FileSystem::ReadFileLinesAsText(configFile);

			for (int32 i = 0; i < lines.Size(); i++)
			{
				// Disregard full line comments
				if (lines[i].BeginsWithAny(";#"))
					continue; 

				// Remove inline comments
				uint32 commentStartIndex = lines[i].FirstIndexOfAny(";#");
				if (commentStartIndex != -1)
					lines[i] = lines[i].RemoveAfter(commentStartIndex - 1);

				List<String> pair = lines[i].Split('=');
				if (pair.Size() != 2) // Line is invalid, so skip
					continue;

				configValues[pair[0]] = pair[1];
			}
			return true;
		}
	};
}