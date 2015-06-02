#pragma once
#include <map>

#include "IntegerDefs.h"
#include "VanguardString.h"
#include "FileSystem.h"

#define CONFIGVAR_STRING(VarName, DefaultValue)

namespace Vanguard
{
	static const FilePath configFile = FileSystem::GetEngineConfigDirectory() + "Engine.cfg";

	class ConfigDefault
	{

	};

	class EngineConfig
	{
		template <class T> class ConfigVar;
		friend class ConfigVar<class T>;

	private:
		static std::map <String, String> configValues;

		static void SetConfigString(const String& aConfigName, const String& aConfigValue)
		{
			configValues[aConfigName] = aConfigValue;
		}

		static void SetConfigBoolean(const String& aConfigName, const bool& aConfigValue)
		{
			configValues[aConfigName] = String::FromBoolean(aConfigValue);
		}

		static void SetConfigFloat(const String& aConfigName, const float& aConfigValue)
		{
			configValues[aConfigName] = String::FromFloat(aConfigValue);
		}

		static void SetConfigInt(const String& aConfigName, const int32& aConfigValue)
		{
			configValues[aConfigName] = String::FromInt32(aConfigValue);
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

		static String GetConfigString(const String& aConfigName, const String& aDefaultValue)
		{
			for (auto const& item : configValues)
			{
				if (item.first == aConfigName)
					return item.second;
			}
			SetConfigString(aConfigName, aDefaultValue);
			return aDefaultValue;
		}

	};
}