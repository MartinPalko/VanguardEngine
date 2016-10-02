#pragma once

#include "Core_Common.h"
#include "Type.h"
#include "WorldObjects/VanguardObject.h"

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

	class VanguardObjectEvent : public WorldEvent
	{
		TYPE_DECLARATION(VanguardObjectEvent, WorldEvent);
		VanguardObject* object;

	public:
		// Need to have default constructor to work with the type system.
		VanguardObjectEvent() : WorldEvent(nullptr)
			, object(nullptr)
		{}

		VanguardObjectEvent(VanguardObject* aObject) : WorldEvent(aObject ? aObject->GetWorld() : nullptr)
			, object(aObject)
		{}

		VanguardObject* GetObject() { return object; }
	};

	class ObjectCreatedEvent : public VanguardObjectEvent
	{
		TYPE_DECLARATION(ObjectCreatedEvent, VanguardObjectEvent);

		ObjectCreatedEvent() : VanguardObjectEvent(nullptr) {}
		ObjectCreatedEvent(VanguardObject* aObject) : VanguardObjectEvent(aObject) {}
	};

	class ObjectDestroyedEvent : public VanguardObjectEvent
	{
		TYPE_DECLARATION(ObjectDestroyedEvent, VanguardObjectEvent);

		ObjectDestroyedEvent() : VanguardObjectEvent(nullptr) {}
		ObjectDestroyedEvent(VanguardObject* aObject) : VanguardObjectEvent(aObject) {}
	};

	class ObjectEnabledEvent : public VanguardObjectEvent
	{
		TYPE_DECLARATION(ObjectEnabledEvent, VanguardObjectEvent);

		ObjectEnabledEvent() : VanguardObjectEvent(nullptr) {}
		ObjectEnabledEvent(VanguardObject* aObject) : VanguardObjectEvent(aObject) {}
	};

	class ObjectDisabledEvent : public VanguardObjectEvent
	{
		TYPE_DECLARATION(ObjectDisabledEvent, VanguardObjectEvent);

		ObjectDisabledEvent() : VanguardObjectEvent(nullptr) {}
		ObjectDisabledEvent(VanguardObject* aObject) : VanguardObjectEvent(aObject) {}
	};
}