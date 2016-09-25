#pragma once

#include "Core_Common.h"

#include "Types/VanguardString.h"
#include "Types/FilePath.h"
#include "Collections/Dictionary.h"

#include <unordered_map>

namespace Vanguard
{
	struct ConfigSection
	{
		friend class ConfigFile;
	private:
		std::unordered_map<StringID, String> entries;
	public:
		ConfigSection() : entries() {}
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

		std::unordered_map<StringID, ConfigSection> sections;

	public:
		ConfigFile();
		~ConfigFile();

		void SetValue(StringID aSection, StringID aKey, const String& aValue);
		bool ContainsValue(StringID aSection, StringID aKey);
		String GetValue(StringID aSection, StringID aKey);
		DynamicArray<String> GetArrayValues(StringID aSection, StringID aKey);

		void LoadAdditive(const FilePath& aConfigFilePath);

		static ConfigFile Load(const FilePath& aConfigFilePath);
	};
}