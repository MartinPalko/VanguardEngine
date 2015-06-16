#pragma once

#include "Config.h"
#include "Foundation.h"
#include "CoreMacros.h"
#include "EntityComponentSystem.h"
#include "NativeReflection.h"
#include "Application.h"
#include "Log.h"

#include "JobManager.h"
#include "Job.h"
#include "Frame.h"

namespace Vanguard
{
	class ModuleManager;
	class ManagedAssembly;

	class CORE_API Core
	{
	private:
		class ModuleManager* moduleManager;
		class ManagedAssembly* managedCore;

	public:
		void Initialize();
		void Run();
		void ShutDown();

		void LoadModule(const String& aModuleName);
	};
}