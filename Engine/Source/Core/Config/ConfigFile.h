#pragma once

#include "Core_Common.h"

#include "Types/VanguardString.h"
#include "Types/FilePath.h"
#include "Collections/Dictionary.h"

namespace Vanguard
{
	struct ConfigSection
	{
		friend class ConfigFile;
	private:
		Dictionary<String, String> entries;
	};

	class CORE_API ConfigFile
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
		ConfigFile();
		~ConfigFile();

		void SetValue(const String& aSection, const String& aKey, const String& aValue);
		bool ContainsValue(const String& aSection, const String& aKey);
		String GetValue(const String& aSection, const String& aKey);
		DynamicArray<String> GetArrayValues(const String& aSection, const String& aKey);

		void LoadAdditive(const FilePath& aConfigFilePath);

		static ConfigFile Load(const FilePath& aConfigFilePath);
	};
}