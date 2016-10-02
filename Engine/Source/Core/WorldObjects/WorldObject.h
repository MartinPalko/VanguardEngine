#pragma once
#include "Foundation.h"
#include "Type.h"

namespace Vanguard
{
	class World;
	class Entity;
	class Component;
	class WorldObjectEvent;

	struct IWorldObjectEventListener
	{
		virtual void OnWorldObjectEvent(WorldObjectEvent* aEvent) = 0;
	};

	class CORE_API WorldObject
	{
		friend World;
		friend Entity;
		friend Component;

		ABSTRACT_BASETYPE_DECLARATION(WorldObject);

		bool pendingDelete;
		DynamicArray<IWorldObjectEventListener*> eventListeners;

	private:
		World* world = nullptr;
		void BroadcastEvent(WorldObjectEvent* aEvent); // Should only be called by world

	public:
		WorldObject();
		virtual ~WorldObject() {}
		World* GetWorld() { return world; }

		virtual void RegisterEventListener(IWorldObjectEventListener* aListener);
		virtual void UnregisterEventListener(IWorldObjectEventListener* aListener);

		virtual void Destroy();
		bool PendingDelete() { return pendingDelete; }
	};
}