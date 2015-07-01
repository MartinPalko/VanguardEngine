#include "Core.h"
#include "IModule.h"
#include "Frame.h"

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

			List<Job*> firstGroup = List <Job*>();
			for (int i = 0; i < 15; i++)
			{
				firstGroup.PushBack(aFrame->AddJob([=]()-> void { UpdatePhysicsThing(aFrame); }));
			}

			for (size_t i = 0; i < firstGroup.Size(); i++)
			{
				aFrame->WaitForJob(firstGroup[i]);
			}

			DEBUG_LOG("NOW DOING SECOND BATCH");
			List<Job*> secondGroup = List <Job*>();
			for (size_t i = 0; i < 15; i++)
			{
				secondGroup.PushBack(aFrame->AddJob([=]()-> void { UpdatePhysicsThing(aFrame); }));
			}

			for (size_t i = 0; i < secondGroup.Size(); i++)
			{
				aFrame->WaitForJob(secondGroup[i]);
			}

			DEBUG_LOG("ALL PHYSICS JOBS DONE");
		}
	};
}