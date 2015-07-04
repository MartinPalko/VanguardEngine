#pragma once

#include "VanguardString.h"
#include "FilePath.h"
#include "Hashtable.h"
#include "INISection.h"

namespace Vanguard
{
	class INIFile
	{
		FilePath filePath;
		Hashtable<INISection> sections;
	};
}