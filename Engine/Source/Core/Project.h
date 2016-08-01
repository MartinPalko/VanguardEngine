#pragma once

#include "Foundation.h"

namespace Vanguard
{
	class Project
	{
		friend class Core;

	protected:
		Project(FilePath aProjectDirectory);

		FilePath projectDirectory;

		String name;
		String friendlyName;
		String description;

	public:
		
		inline String GetName(){ return name; }
		inline String GetFriendlyName(){ return friendlyName; }
		inline String GetDescription(){ return description; }
	};
}