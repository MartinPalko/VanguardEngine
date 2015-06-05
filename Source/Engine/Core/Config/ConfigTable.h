#pragma once
#include <map>

#include "CoreMacros.h"
#include "IntegerDefs.h"
#include "VanguardString.h"
#include "FileSystem.h"

#include "ConfigVar.h"

namespace Vanguard
{
	class IConfigVar;
	template <class T> class ConfigVar;

	class CORE_API ConfigTable
	{
		friend class ConfigVar<class T>;

	private:
		// Map goes <File,<Section,<Name,Value>>>
		static std::map <String, std::map <String, std::map <String, String>>> configValues;

	public:
		static void OnConfigVarCreated(const IConfigVar& aNewVar, const String& aConfigDefault);
		static String GetConfigValueText(const IConfigVar& aConfigVar);

	public:
		static bool SaveConfigToDisk();
		static bool LoadConfigFromDisk();
	};
}