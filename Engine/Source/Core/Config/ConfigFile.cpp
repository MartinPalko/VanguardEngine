#include "ConfigFile.h"
#include "Utility/FileSystem.h"
#include "Log.h"

namespace Vanguard
{
	ConfigFile::ConfigFile()
		: sections()
	{
	}

	ConfigFile::~ConfigFile()
	{
	}

	void ConfigFile::SetValue(const String& aSection, const String& aKey, const String& aValue)
	{
		sections[aSection].entries[aKey] = aValue;
	}

	bool ConfigFile::ContainsValue(const String& aSection, const String& aKey)
	{
		return (sections.count(aSection) && sections[aSection].entries.count(aKey));
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

	DynamicArray<String> ConfigFile::GetArrayValues(const String& aSection, const String& aKey)
	{
		String value = GetValue(aSection, aKey);
		return value.Split(',');
	}

	void ConfigFile::LoadAdditive(const FilePath& aConfigFilePath)
	{
		if (!FileSystem::FileExists(aConfigFilePath))
		{
			DEBUG_ERROR("Cannot load config file at path: " + aConfigFilePath.GetFullPathName());
			return;
		}

		DynamicArray<String> lines = FileSystem::ReadFileLinesAsText(aConfigFilePath);

		StringID currentSection = "";

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
				currentSection = lineText.Trim("[]");
				continue;
			}

			DynamicArray<String> pair = lineText.Split('=');

			if (pair.Count() != 2) // Line is invalid, so skip
			{
				DEBUG_WARN("Could not parse line " + String::FromInt(i) + " in " + aConfigFilePath.GetFullPathName());
				continue;
			}

			String keyString = pair[0];
			String value = pair[1];

			// Determine if we're doing an addition or removal.
			ConfigOperator operation = ConfigOperator::Set;

			if (keyString.EndsWith('+'))
			{
				operation = ConfigOperator::Add;
				keyString = keyString.TrimEnd('+');
			}
			else if (keyString.EndsWith('-'))
			{
				operation = ConfigOperator::Remove;
				keyString = keyString.TrimEnd('-');
			}

			// Make sure to trim whitespace AFTER we check for operators.
			StringID key = keyString.Trim();
			value = value.Trim();

			bool keyExists = sections[currentSection].entries.count(key) == 0;

			if (operation == ConfigOperator::Add && keyExists) // Append value
			{
				sections[currentSection].entries[key] += ',' + value;
			}
			else if (operation == ConfigOperator::Remove)
			{
				if (!keyExists)
				{
					DEBUG_WARN("Trying to remove value " + value + " from key" + keyString + "when " + keyString + " does not exist");
					continue;
				}

				DynamicArray<String> existingValues = sections[currentSection].entries[key].Split(',');
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
					if (!sections[currentSection].entries.erase(StringID(key)))
					{
						bool thefuck = true;
					}
				}
			}
			else // Just set the value.
			{
				sections[currentSection].entries[key] = value;
			}
		}
	}

	ConfigFile ConfigFile::Load(const FilePath& aConfigFilePath)
	{
		ConfigFile newConfig = ConfigFile();
		newConfig.LoadAdditive(aConfigFilePath);
		return newConfig;
	}
}