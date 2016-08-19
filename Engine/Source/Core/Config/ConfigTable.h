#pragma once
#include <map>

#include "Core_Common.h"
#include "Config/ConfigFile.h"

namespace Vanguard
{
	class ConfigVar;
	template <class T> class ConfigVar_T;

	class CORE_API ConfigTable
	{
		friend class ConfigVar_T<class T>;

	public:
		static void OnConfigVarCreated(const ConfigVar& aNewVar, const String& aConfigDefault);
		static String GetConfigValueText(const ConfigVar& aConfigVar);

	public:
		static void LoadConfigFromDisk();
	};
}
