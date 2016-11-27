#pragma once

#include "Core_Common.h"
#include "Type.h"
#include "Foundation_Common.h"

namespace Vanguard
{
	class Entity;
	class Frame;
	class WorldObject;
	class World;

	class CORE_API Event
	{
		ABSTRACT_BASETYPE_DECLARATION(Event);

		virtual ~Event() {}
	};

	class CORE_API WorldAddedEvent : public Event
	{
		TYPE_DECLARATION(WorldAddedEvent, Event);
		World* world;

	public:
		// Need to have default constructor to work with the type system.
		WorldAddedEvent() : WorldAddedEvent(nullptr) {}
		WorldAddedEvent(World* aWorld) : Event()
			, world(aWorld)
		{}

		World* GetWorld() { return world; }
	};

	class CORE_API WorldRemovedEvent : public Event
	{
		TYPE_DECLARATION(WorldRemovedEvent, Event);
		World* world;

	public:
		// Need to have default constructor to work with the type system.
		WorldRemovedEvent() : WorldRemovedEvent(nullptr) {}
		WorldRemovedEvent(World* aWorld) : Event()
			, world(aWorld)
		{}

		World* GetWorld() { return world; }
	};

	class CORE_API WorldEvent : public Event
	{
		ABSTRACT_TYPE_DECLARATION(WorldEvent, Event);
		World* world;

	public:
		WorldEvent(World* aWorld) : Event()
			, world(aWorld)
		{}

		World* GetWorld() { return world; }
	};

	class CORE_API PreTickEvent : public WorldEvent
	{
		TYPE_DECLARATION(PreTickEvent, WorldEvent);

	public:
		// Need to have default constructor to work with the type system.
		PreTickEvent() : PreTickEvent(nullptr) {}
		PreTickEvent(World* aWorld);
	};

	class CORE_API PostTickEvent : public WorldEvent
	{
		TYPE_DECLARATION(PostTickEvent, WorldEvent);

	public:
		// Need to have default constructor to work with the type system.
		PostTickEvent() : PostTickEvent(nullptr) {}
		PostTickEvent(World* aWorld);
	};

	class CORE_API ObjectAddedEvent : public WorldEvent
	{
		TYPE_DECLARATION(ObjectAddedEvent, WorldEvent);
		WorldObject* object;

	public:
		// Need to have default constructor to work with the type system.
		ObjectAddedEvent() : ObjectAddedEvent(nullptr) {}
		ObjectAddedEvent(WorldObject* aObject);

		WorldObject* GetObject() { return object; }
	};

	class CORE_API ObjectRemovedEvent : public WorldEvent
	{
		TYPE_DECLARATION(ObjectRemovedEvent, WorldEvent);
		WorldObject* object;

	public:
		// Need to have default constructor to work with the type system.
		ObjectRemovedEvent() : ObjectRemovedEvent(nullptr) {}
		ObjectRemovedEvent(WorldObject* aObject);

		WorldObject* GetObject() { return object; }
	};

	class CORE_API WorldObjectEvent : public WorldEvent
	{
		TYPE_DECLARATION(WorldObjectEvent, WorldEvent);
		WorldObject* object;

	public:
		// Need to have default constructor to work with the type system.
		WorldObjectEvent() : WorldEvent(nullptr)
			, object(nullptr)
		{}

		WorldObjectEvent(WorldObject* aObject);

		WorldObject* GetObject() { return object; }
	};

	class CORE_API ObjectCreatedEvent : public WorldObjectEvent
	{
		TYPE_DECLARATION(ObjectCreatedEvent, WorldObjectEvent);

		ObjectCreatedEvent() : WorldObjectEvent(nullptr) {}
		ObjectCreatedEvent(WorldObject* aObject) : WorldObjectEvent(aObject) {}
	};

	class CORE_API ObjectDestroyedEvent : public WorldObjectEvent
	{
		TYPE_DECLARATION(ObjectDestroyedEvent, WorldObjectEvent);

		ObjectDestroyedEvent() : WorldObjectEvent(nullptr) {}
		ObjectDestroyedEvent(WorldObject* aObject) : WorldObjectEvent(aObject) {}
	};

	class CORE_API ObjectEnabledEvent : public WorldObjectEvent
	{
		TYPE_DECLARATION(ObjectEnabledEvent, WorldObjectEvent);

		ObjectEnabledEvent() : WorldObjectEvent(nullptr) {}
		ObjectEnabledEvent(WorldObject* aObject) : WorldObjectEvent(aObject) {}
	};

	class CORE_API ObjectDisabledEvent : public WorldObjectEvent
	{
		TYPE_DECLARATION(ObjectDisabledEvent, WorldObjectEvent);

		ObjectDisabledEvent() : WorldObjectEvent(nullptr) {}
		ObjectDisabledEvent(WorldObject* aObject) : WorldObjectEvent(aObject) {}
	};
}