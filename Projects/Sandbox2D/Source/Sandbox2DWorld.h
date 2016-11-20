#pragma once
#include "Sandbox2D_Common.h"
#include "Renderer/RenderView.h"

namespace gainput
{
	class InputManager;
	class InputMap;
}

namespace Sandbox2D
{
	class Sandbox2DCamera;

	class Sandbox2DWorld : public World, public INativeEventHandler
	{
		TYPE_DECLARATION(Sandbox2DWorld, World)

	private:
		Sandbox2DCamera* playerCamera;
		RenderView* playerView;

		gainput::InputManager* inputManager;
		gainput::InputMap* inputMap;

	public:
		Sandbox2DWorld();
		~Sandbox2DWorld();

		// Override World
	protected:
		virtual void Tick(Frame* aFrame) override;

		// Implement INativeEventHandler
	public:
		virtual void HandleNativeEvent(NativeEvent aEvent) override;
	};
}