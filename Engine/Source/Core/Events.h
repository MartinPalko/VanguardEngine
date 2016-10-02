#pragma once

#include "Core_Common.h"
#include "Type.h"
#include "WorldObjects/WorldObject.h"

namespace Vanguard
{
	class Entity;

	class Event
	{
		ABSTRACT_BASETYPE_DECLARATION(Event);

		virtual ~Event() {}
	};

	class WorldEvent : public Event
	{
		ABSTRACT_TYPE_DECLARATION(WorldEvent, Event);
		World* world;

	public:
		WorldEvent(World* aWorld) : Event()
			, world(aWorld)
		{}

		World* GetWorld() { return world; }
	};

	class WorldObjectEvent : public WorldEvent
	{
		TYPE_DECLARATION(WorldObjectEvent, WorldEvent);
		WorldObject* object;

	public:
		// Need to have default constructor to work with the type system.
		WorldObjectEvent() : WorldEvent(nullptr)
			, object(nullptr)
		{}

		WorldObjectEvent(WorldObject* aObject) : WorldEvent(aObject ? aObject->GetWorld() : nullptr)
			, object(aObject)
		{}

		WorldObject* GetObject() { return object; }
	};

	class ObjectCreatedEvent : public WorldObjectEvent
	{
		TYPE_DECLARATION(ObjectCreatedEvent, WorldObjectEvent);

		ObjectCreatedEvent() : WorldObjectEvent(nullptr) {}
		ObjectCreatedEvent(WorldObject* aObject) : WorldObjectEvent(aObject) {}
	};

	class ObjectDestroyedEvent : public WorldObjectEvent
	{
		TYPE_DECLARATION(ObjectDestroyedEvent, WorldObjectEvent);

		ObjectDestroyedEvent() : WorldObjectEvent(nullptr) {}
		ObjectDestroyedEvent(WorldObject* aObject) : WorldObjectEvent(aObject) {}
	};

	class ObjectEnabledEvent : public WorldObjectEvent
	{
		TYPE_DECLARATION(ObjectEnabledEvent, WorldObjectEvent);

		ObjectEnabledEvent() : WorldObjectEvent(nullptr) {}
		ObjectEnabledEvent(WorldObject* aObject) : WorldObjectEvent(aObject) {}
	};

	class ObjectDisabledEvent : public WorldObjectEvent
	{
		TYPE_DECLARATION(ObjectDisabledEvent, WorldObjectEvent);

		ObjectDisabledEvent() : WorldObjectEvent(nullptr) {}
		ObjectDisabledEvent(WorldObject* aObject) : WorldObjectEvent(aObject) {}
	};
}