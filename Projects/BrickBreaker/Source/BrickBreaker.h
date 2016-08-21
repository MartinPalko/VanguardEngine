#pragma once
#include "BrickBreaker_Common.h"
#include "Modules/GameModule.h"

#include "SpriteComponent.h"

#include "Renderer/RenderView.h"

namespace gainput
{
	class InputManager;
	class InputMap;
}

namespace BrickBreaker
{
	class BrickBreaker : public GameModule, public INativeEventHandler
	{
	private:
		WindowHandle gameWindow;
		RenderView* playerView;
		Entity* paddle;

		gainput::InputManager* inputManager;
		gainput::InputMap* inputMap;
		
		// Implement GameModule
	protected:
		virtual void SetupGame() override;
		virtual void UpdateGame(Frame* aFrame) override;
		virtual void CleanupGame() override;

		// Implement INativeEventHandler
	public:
		virtual void HandleNativeEvent(NativeEvent aEvent) override;
	};
}