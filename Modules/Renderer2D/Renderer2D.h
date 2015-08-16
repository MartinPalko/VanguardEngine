#include "Renderer2D_Common.h"
#include "Modules/IModule.h"

#include "SpriteComponent.h"


namespace Vanguard
{
	class Renderer2D : public IModule
	{
		virtual String GetModuleName() { return "Renderer2D"; }
		virtual String GetModuleType() { return "Renderer"; }

		SpriteComponent* sprite;

		virtual void LoadModule()
		{
			DEBUG_LOG("Loaded Renderer2D");
			sprite = new SpriteComponent();
		}

		virtual void UnloadModule()
		{
			DEBUG_LOG("Unloaded Renderer2D");
			delete sprite;
		}

	};
}