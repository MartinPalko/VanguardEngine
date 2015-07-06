#include "ConfigFile.h"

namespace Vanguard
{

	void ConfigFile::SetValue(const String& aSection, const String& aKey, const String& aValue)
	{
		sections[aSection].entries[aKey] = aValue;
	}

	bool ConfigFile::ContainsValue(const String& aSection, const String& aKey)
	{
		return (sections.Contains(aSection) && sections[aSection].entries.Contains(aKey));
	}

	String ConfigFile::GetValue(const String& aSection, const String& aKey)
	{
		if (ContainsValue(aSection, aKey))
		{
			return sections[aSection].entries[aKey];
		}
		else
		{
			DEBUG_ERROR("Config file does not contain " + aKey + " in section " + aSection)
				return "";
		}
	}

	void ConfigFile::LoadAdditive(FilePath aConfigFilePath)
	{
		if (!FileSystem::FileExists(aConfigFilePath))
		{
			DEBUG_ERROR("Cannot load config file at path: " + aConfigFilePath.GetFullPathName());
			return;
		}

		DynamicArray<String> lines = FileSystem::ReadFileLinesAsText(aConfigFilePath);

		String CurrentSection = "";

		for (uint32 i = 0; i < lines.Count(); i++)
		{
			String lineText = lines[i];

			// Disregard full line comments
			if (lineText.BeginsWithAny(";#"))
				continue;

			// Remove inline comments
			size_t commentStartIndex = lineText.FirstIndexOfAny(";#");
			if (commentStartIndex != String::NPos())
				lineText = lineText.RemoveAfter(commentStartIndex - 1);

			// Section marker, begin new section.
			if (lineText.BeginsWith('['))
			{
				CurrentSection = lineText.Trim("[]");
				continue;
			}

			DynamicArray<String> pair = lineText.Split('=');

			if (pair.Count() != 2) // Line is invalid, so skip
			{
				DEBUG_WARN("Could not parse line " + String::FromInt32(i) + " in " + aConfigFilePath.GetFullPathName());
				continue;
			}

			String key = pair[0];
			String value = pair[1];

			// Determine if we're doing an addition or removal.
			ConfigOperator operation = ConfigOperator::Set;

			if (key.EndsWith('+'))
			{
				operation = ConfigOperator::Add;
				key = key.TrimEnd('+');
			}
			else if (key.EndsWith('-'))
			{
				operation = ConfigOperator::Remove;
				key = key.TrimEnd('-');
			}

			bool keyExists = sections[CurrentSection].entries.Contains(key);

			if (operation == ConfigOperator::Add && keyExists) // Append value
			{
				sections[CurrentSection].entries[key] += ',' + value;
			}
			else if (operation == ConfigOperator::Remove)
			{
				if (!keyExists)
				{
					DEBUG_WARN("Trying to remove value " + value + " from key" + key + "when " + key + " does not exist");
					continue;
				}

				DynamicArray<String> existingValues = sections[CurrentSection].entries[key].Split(',');
				existingValues.Remove(value);

				if (existingValues.Count() > 0)
				{
					String returnValue = existingValues[0];
					for (size_t i = 1; i < existingValues.Count(); i++)
					{
						returnValue += ',' + existingValues[i];
					}
				}
				else
				{
					// No more values, remove key.
					sections[CurrentSection].entries.Remove(key);
				}
			}
			else // Just set the value.
			{
				sections[CurrentSection].entries[key] = value;
			}
		}
	}

	ConfigFile ConfigFile::Load(FilePath aConfigFilePath)
	{
		ConfigFile newConfig = ConfigFile();
		newConfig.LoadAdditive(aConfigFilePath);
		return newConfig;
	}
}