#pragma once
#include "Physics2D_Common.h"
#include "Interfaces/IModule.h"
#include "Renderer/IRenderer.h"

#include "RigidBody2D.h"

namespace Vanguard
{
	class PhysicsWorld2D;

	class Physics2D : public IModule, protected ICoreEventListener
	{
	protected:
		std::unordered_map<World*, PhysicsWorld2D*> physicsWorlds;

	public:
		Physics2D();
		~Physics2D();

		// IModule Implementation
		virtual void LoadModule() override;
		virtual void UnloadModule() override;

		// ICoreEventListener Implementation
		virtual void OnCoreEvent(Event* aEvent) override;

	};
}