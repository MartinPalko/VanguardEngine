#include "Core.h"
#include <ostream>

#include "ManagedClass.h"
#include "ModuleManager.h"
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

		std::cout << "Initialized Core" << "\n\n";
	}

	void PrintChildrenRecursively(INativeClassInfo* Class, int currentDepth)
	{
		for (int i = 0; i < currentDepth; i++)
		{
			std::cout << "\t";
		}
		std::cout << Class->GetTypeName() << "\n";

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
		std::cout << "Running Core" << "\n\n";

		List<INativeClassInfo*> allTypes = INativeClassInfo::GetAllTypes();
		std::cout << allTypes.Size() << " types found: \n";
		for (int i = 0; i < allTypes.Size(); i++)
		{
			if (allTypes[i]->GetBaseClass() == nullptr)
				PrintChildrenRecursively(allTypes[i], 0);
		}
		std::cout << "\n";

		World* gameWorld = new World();
		Transform* transform = new Transform();

		
		Frame* frame = new Frame(0, 0.03f, gameWorld);

		for (int i = 0; i < 50; i++)
		{
			frame->AddJob(TestJobFunction);
		}
		JobManager::ProcessFrame(frame);


		delete transform;
		delete gameWorld;
		delete frame;
		std::cout << "Ran Core" << "\n\n";
	}

	void Core::ShutDown()
	{
		delete moduleManager;
		delete managedCore;

		std::cout << "Shut Down Core" << "\n\n";

		ConfigTable::SaveConfigToDisk();
	}

	void Core::LoadModule(const String& aModuleName)
	{
		moduleManager->LoadModule(aModuleName);
	}
}
