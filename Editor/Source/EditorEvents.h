#pragma once

#include "Editor_Common.h"

namespace Vanguard
{
	class EditorEvent : public Event
	{
		ABSTRACT_TYPE_DECLARATION(EditorEvent, Event);
	};

	class EntitySelectedEvent : public EditorEvent
	{
		TYPE_DECLARATION(EntitySelectedEvent, EditorEvent);
		Entity* entity;

	public:
		EntitySelectedEvent() : EntitySelectedEvent(nullptr) {}
		EntitySelectedEvent(Entity* aEntity) : EditorEvent()
			, entity(aEntity)
		{}

		Entity* GetEntity() { return entity; }
	};

	class EntityUnselectedEvent : public EditorEvent
	{
		TYPE_DECLARATION(EntityUnselectedEvent, EditorEvent);
		Entity* entity;

	public:
		EntityUnselectedEvent() : EntityUnselectedEvent(nullptr) {}
		EntityUnselectedEvent(Entity* aEntity) : EditorEvent()
			, entity(aEntity)
		{}

		Entity* GetEntity() { return entity; }
	};
}