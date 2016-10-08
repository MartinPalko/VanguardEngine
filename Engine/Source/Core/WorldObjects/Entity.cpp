#include "Entity.h"
#include "Component.h"
#include "World.h"
#include "Transform.h"
#include "Events.h"

namespace Vanguard
{
	TYPE_DEFINITION(Entity, WorldObject)

	START_REGISTER_PROPERTIES(Entity);
	{
		REGISTER_PROPERTY(Entity, uuid);
		REGISTER_PROPERTY(Entity, enabled);
	}
	FINISH_REGISTER_PROPERTIES(Entity);

	Entity::Entity() : WorldObject()
		, uuid()
		, name()
		, parent(nullptr)
		, children()
		, enabled(true)
		, tickEnabled (false)
		, tickRegistered (false)
	{
	}

	Entity::~Entity()
	{
	}

	void Entity::Destroy()
	{
		WorldObject::Destroy();
		for (auto component : components)
		{
			component->Destroy();
		}
	}

	void Entity::OnWorldObjectEvent(WorldObjectEvent* aEvent)
	{
		if (!PendingDelete() && aEvent->GetType()->IsA<ObjectDestroyedEvent>())
		{
			components.Remove((Component*)aEvent->GetObject());
			componentTypeMap.erase(aEvent->GetType()->GetRuntimeHash());
		}
	}

	Component* Entity::GetComponent(Type* aComponentType)
	{
		Component* foundComponent = componentTypeMap[aComponentType->GetRuntimeHash()];

		if (!foundComponent)
		{
			// Check inherited classes.
			DynamicArray<Type*> derivedClasses = aComponentType->GetDerivedClasses();
			for (int i = 0; i < derivedClasses.Count(); i++)
			{
				foundComponent = GetComponent(derivedClasses[i]);
				if (foundComponent)
					break;
			}
		}

		return foundComponent;
	}

	Component* Entity::AddComponent(Type* aComponentType)
	{
		Type* componentType = Type::GetType<Component>();

		if (!aComponentType->IsA(componentType))
		{
			LOG_WARNING("Cannot add component. Class of type " + aComponentType->GetTypeName() + " does not derive from Component", "Entity");
			return nullptr;
		}

		Component* newComponent = (Component*)aComponentType->CreateInstance();

		newComponent->entity = this;
		if (GetWorld())
		{
			GetWorld()->RegisterObject(newComponent);
			GetWorld()->PostEvent(new ObjectCreatedEvent(newComponent));
		}
		components.PushBack(newComponent);
		componentTypeMap[aComponentType->GetRuntimeHash()] = newComponent;

		ComponentAdded(newComponent);

		newComponent->RegisterEventListener(this);

		return newComponent;
	}

	void Entity::SetEnabled(bool aEnabled)
	{
		if (aEnabled != enabled && tickEnabled && GetWorld())
		{
			if (aEnabled && !tickRegistered)
			{
				GetWorld()->RegisterTick(this);
				tickRegistered = true;
			}
			else if (!aEnabled && tickRegistered)
			{
				GetWorld()->UnregisterTick(this);
				tickRegistered = false;
			}

			enabled = aEnabled;
		}
	}

	void Entity::EnableTick()
	{
		if (!tickRegistered)
		{
			if (GetWorld())
			{
				GetWorld()->RegisterTick(this);
				tickRegistered = true;
			}
			tickEnabled = true;
		}
	}

	void Entity::DisableTick()
	{
		if (GetWorld())
		{
			GetWorld()->UnregisterTick(this);
			tickRegistered = false;
		}
		tickEnabled = false;
	}
}