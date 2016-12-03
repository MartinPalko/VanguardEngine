#pragma once
#include "Core_Common.h"

namespace Vanguard
{
	class CORE_API ISubsystem
	{
	public:
		virtual ~ISubsystem(){}

		// Return true if this subsytem requires service. Otherwise, the out variable should be filled with the estimated time that this subsystem will require service.
		virtual bool NeedsService(Timespan aCurrentTime, Timespan aLastServiced, Timespan& outEstNextServiceTime) = 0;
		// Service this subsystem
		virtual void ServiceSubsystem(Timespan aCurrentTime) = 0;
	};
}
