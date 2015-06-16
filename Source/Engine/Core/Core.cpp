#include "Core.h"
#include <ostream>

#include "ManagedClass.h"
#include "ModuleManager.h"
#include "IModule.h"
#include "ManagedAssembly.h"
#include "NativeReflection.h"
#include "JobManager.h"
#include "Job.h"
#include "Frame.h"

namespace Vanguard
{
	void Core::Initialize()
	{
		ConfigTable::LoadConfigFromDisk();
		ConfigTable::SaveConfigToDisk(); // Save right away, to generate defaults if they don't exist. TODO: More elequently

		JobManager::CreateThreads();

		managedCore = new ManagedAssembly("ManagedCore");
		moduleManager = new ModuleManager(managedCore);

		Log::Write("Initialized Core");
	}

	void PrintChildrenRecursively(INativeClassInfo* Class, int currentDepth)
	{
		String logmessage;
		for (int i = 0; i < currentDepth; i++)
		{
			logmessage+= "\t";
		}
		logmessage += Class->GetTypeName();
		Log::Write(logmessage);

		List<INativeClassInfo*> children = Class->GetDerivedClasses();
		for (int i = 0; i < children.Size(); i++)
			PrintChildrenRecursively(children[i], currentDepth + 1);
	}

	void TestJobFunction(Frame* aFrame)
	{
		for (int i = 0; i < 10000000; i++)
		{
			i--;
			i++;
		}
	}

	void Core::Run()
	{
		Log::Write("Running Core");

		List<INativeClassInfo*> allTypes = INativeClassInfo::GetAllTypes();
		Log::Write(allTypes.Size() + " types found:");
		for (int i = 0; i < allTypes.Size(); i++)
		{
			if (allTypes[i]->GetBaseClass() == nullptr)
				PrintChildrenRecursively(allTypes[i], 0);
		}

		World* gameWorld = new World();
		Transform* transform = new Transform();


		Frame* frame = new Frame(0, 0.03f, gameWorld);

		List<IModule*> loadedModules = moduleManager->GetLoadedModules();

		for (int i = 0; i < loadedModules.Size(); i++)
		{
			frame->AddJob([=]()-> void { loadedModules[i]->OnFrame(frame); });
		}

		JobManager::ProcessFrame(frame);

		delete transform;
		delete gameWorld;
		delete frame;
		Log::Write("Ran Core");
	}

	void Core::ShutDown()
	{
		delete moduleManager;
		delete managedCore;

		Log::Write("Shut Down Core");
		ConfigTable::SaveConfigToDisk();
	}

	void Core::LoadModule(const String& aModuleName)
	{
		moduleManager->LoadModule(aModuleName);
	}
}
