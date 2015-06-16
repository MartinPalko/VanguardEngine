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
			Log::Write("Update physics thing");
		}

		virtual void OnFrame(Frame* aFrame) override
		{
			Log::Write("PHYSICS ON FRAME");

			List<Job*> firstGroup = List <Job*>();
			for (int i = 0; i < 15; i++)
			{
				firstGroup.PushBack(aFrame->AddJob([=]()-> void { UpdatePhysicsThing(aFrame); }));
			}

			for (int i = 0; i < firstGroup.Size(); i++)
			{
				aFrame->WaitForJob(firstGroup[i]);
			}

			Log::Write("NOW DOING SECOND BATCH");
			List<Job*> secondGroup = List <Job*>();
			for (int i = 0; i < 15; i++)
			{
				secondGroup.PushBack(aFrame->AddJob([=]()-> void { UpdatePhysicsThing(aFrame); }));
			}

			for (int i = 0; i < secondGroup.Size(); i++)
			{
				aFrame->WaitForJob(secondGroup[i]);
			}

			Log::Write("ALL PHYSICS JOBS DONE");
		}
	};
}