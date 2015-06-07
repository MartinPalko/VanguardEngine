#include "Core.h"
#include "IModule.h"

namespace Vanguard
{
	class PhysXModule : public IModule
	{
		virtual String GetPluginName() { return "PhysX"; }
		virtual String GetPluginType() { return "Physics"; }
	};
}