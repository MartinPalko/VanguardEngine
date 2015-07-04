#pragma once

#include "Log.h"
#include "VanguardString.h"
#include "FileSystem.h"
#include "FilePath.h"
#include "Hashtable.h"
#include "ConfigSection.h"

namespace Vanguard
{
	class ConfigFile
	{
	private:
		FilePath filePath;
		Dictionary<String,ConfigSection> sections;

		ConfigFile(FilePath aPath) : filePath(aPath) {}

	public:

		static ConfigFile Load(FilePath aConfigFilePath)
		{
			ConfigFile newConfig(aConfigFilePath);


			if (!FileSystem::FileExists(aConfigFilePath))
			{
				DEBUG_ERROR("Cannot load config file at path: " + aConfigFilePath.GetFullPathName());
				return newConfig;
			}


			List<String> lines = FileSystem::ReadFileLinesAsText(aConfigFilePath);

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
				{
					DEBUG_WARN("Could not parse line " + String::FromInt32(i) + " in " + aConfigFilePath.GetFullPathName());
					continue;
				}

				newConfig.sections[CurrentSection].entries[pair[0]] = pair[1];
			}
		}
		
	};
}