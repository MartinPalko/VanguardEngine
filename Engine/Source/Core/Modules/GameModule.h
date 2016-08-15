#pragma once
#include "IModule.h"

#include "Types/VanguardTime.h"

namespace Vanguard
{
	class CORE_API GameModule : public IModule
	{
	private:
		World* gameWorld;

	public:
		GameModule();

		// From IModule
		virtual void LoadModule() override;
		virtual void UnloadModule() override;

	protected:
		virtual void SetupGame() {};
		virtual void UpdateGame(Frame* aFrame) {};
		virtual void CleanupGame() {};
	};
}
