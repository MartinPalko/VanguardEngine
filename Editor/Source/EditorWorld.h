#pragma once
#include "Editor_Common.h"

namespace Vanguard
{
	class EditorWorld : public World, public INativeEventHandler
	{
	public:
		EditorWorld();
		~EditorWorld();

		// Override World
	protected:
		virtual void Tick(Frame* aFrame) override;

		// Implement INativeEventHandler
	public:
		virtual void HandleNativeEvent(NativeEvent aEvent) override;
	};
}