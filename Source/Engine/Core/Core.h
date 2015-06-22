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
		static Core* instance;

		class ModuleManager* moduleManager;
		class ManagedAssembly* managedCore;

		bool exiting = false;

	public:
		static Core* GetInstance();

		void Initialize();
		void Run();
		void ShutDown();

		// Sets exiting to true, will exit the main run() loop when appropriate.
		void Exit()
		{
			exiting = true; 
		}

		void LoadModule(const String& aModuleName);
	};
}