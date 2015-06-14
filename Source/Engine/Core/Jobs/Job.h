#pragma once
#include "JobEnums.h"

namespace Vanguard
{
	class Frame;
	typedef void(*jobEntryPoint)(Frame*);

	class Job
	{
	private:
		Frame* frame;
		bool running;

		jobEntryPoint entryPoint;
	public:
		Job(jobEntryPoint aEntryPoint, Frame* aFrame)
		{
			entryPoint = aEntryPoint;
			frame = aFrame;
		}

		void Execute()
		{
			entryPoint(frame);
		}
	};
}