#pragma once
#include <map>

#include "Core_Common.h"
#include "VanguardString.h"
#include "FileSystem.h"
#include "ConfigFile.h"

namespace Vanguard
{
	class IConfigVar;
	template <class T> class ConfigVar;

	class CORE_API ConfigTable
	{
		friend class ConfigVar<class T>;

	private:
		// Map goes <File,<Section,<Name,Value>>>
		static Dictionary<String, ConfigFile> configFiles;

	public:
		static void OnConfigVarCreated(const IConfigVar& aNewVar, const String& aConfigDefault);
		static String GetConfigValueText(const IConfigVar& aConfigVar);

	public:
		static void LoadConfigFromDisk();
	};
}
