#include "Core.h"
#include "IModule.h"

namespace Vanguard
{
	class PhysXModule : public IModule
	{
		virtual String GetModuleName() { return "PhysX"; }
		virtual String GetModuleType() { return "Physics"; }
	};
}