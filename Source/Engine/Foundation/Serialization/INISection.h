#pragma once

#include "VanguardString.h"
#include "Hashtable.h"
#include "INIEntry.h"

namespace Vanguard
{
	class INISection
	{
		String name;
		Hashtable<INIEntry> entries;
	};
}