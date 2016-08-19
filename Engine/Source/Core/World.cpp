#include "World.h"

#include "Jobs/Frame.h"

namespace Vanguard
{
	World::World(String aWorldName)
		: worldName(aWorldName)
		, entities()
		, objects()
		, nextFrameNumber(0)
		, lastTickStartTime(0.0)
		, minimumTickDelta(1.0 / 60.0) // 60 FPS
		, registeredTicks()
	{
	}

	World::~World()
	{
		for (int i = 0; i < objects.Count(); i++)
		{
			// We own these objects, so clean them up.
			objects[i]->world = nullptr;
			delete objects[i];
		}
	}

	void World::Tick(Frame* aFrame)
	{
		DynamicArray<Job*> jobs;

		// Dispatch all ticks to the job system.
		for (size_t i = 0; i < registeredTicks.Count(); i++)
		{
			std::function<void(Frame*)> tickFunction = registeredTicks[i];
			jobs.PushBack(aFrame->AddJob([tickFunction, aFrame]()-> void {tickFunction(aFrame); }));
		}

		// Wait for all jobs to complete.
		for (size_t i = 0; i < jobs.Count(); i++)
		{
			aFrame->WaitForJob(jobs[i]);
		}
	}
}
