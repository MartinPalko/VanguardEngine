#pragma once

#include "Foundation.h"
#include "Interfaces/IModule.h"

namespace Vanguard
{
	class World;

	class ProjectModule : public IModule
	{
	public:
		virtual World* CreateWorld() = 0;
	};

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