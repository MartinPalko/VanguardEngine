#pragma once

#include "VanguardString.h"
#include "Dictionary.h"

namespace Vanguard
{
	struct ConfigSection
	{
		friend class ConfigFile;
	private:
		Dictionary<String,String> entries;
	};
}