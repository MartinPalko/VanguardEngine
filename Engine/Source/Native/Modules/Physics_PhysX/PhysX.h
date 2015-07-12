#include "Core.h"
#include "Modules/IModule.h"
#include "Jobs/Frame.h"

namespace Vanguard
{
	class PhysXModule : public IModule
	{
		virtual String GetModuleName() { return "PhysX"; }
		virtual String GetModuleType() { return "Physics"; }

		virtual void UpdatePhysicsThing(Frame* aFrame)
		{
			DEBUG_LOG("Update physics thing");
		}

		virtual void OnFrame(Frame* aFrame) override
		{
			DEBUG_LOG("PHYSICS ON FRAME");

			DynamicArray<Job*> firstGroup = DynamicArray <Job*>();
			for (int i = 0; i < 15; i++)
			{
				firstGroup.PushBack(aFrame->AddJob([=]()-> void { UpdatePhysicsThing(aFrame); }));
			}

			for (size_t i = 0; i < firstGroup.Count(); i++)
			{
				aFrame->WaitForJob(firstGroup[i]);
			}

			DEBUG_LOG("NOW DOING SECOND BATCH");
			DynamicArray<Job*> secondGroup = DynamicArray <Job*>();
			for (size_t i = 0; i < 15; i++)
			{
				secondGroup.PushBack(aFrame->AddJob([=]()-> void { UpdatePhysicsThing(aFrame); }));
			}

			for (size_t i = 0; i < secondGroup.Count(); i++)
			{
				aFrame->WaitForJob(secondGroup[i]);
			}

			DEBUG_LOG("ALL PHYSICS JOBS DONE");
		}
	};
}