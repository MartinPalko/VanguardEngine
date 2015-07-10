#pragma once

#include "Log.h"
#include "VanguardString.h"
#include "FileSystem.h"
#include "FilePath.h"
#include "Hashtable.h"

namespace Vanguard
{
	struct ConfigSection
	{
		friend class ConfigFile;
	private:
		Dictionary<String, String> entries;
	};

	class ConfigFile
	{
	private:
		enum class ConfigOperator
		{
			Set,
			Add,
			Remove
		};

		Dictionary<String, ConfigSection> sections;

	public:
		ConfigFile() {}

		void SetValue(const String& aSection, const String& aKey, const String& aValue);
		bool ContainsValue(const String& aSection, const String& aKey);
		String GetValue(const String& aSection, const String& aKey);
		DynamicArray<String> GetArrayValues(const String& aSection, const String& aKey);

		void LoadAdditive(FilePath aConfigFilePath);

		static ConfigFile Load(FilePath aConfigFilePath);		
	};
}