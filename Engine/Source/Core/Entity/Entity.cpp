#include "Entity.h"
#include "Component.h"
#include "World.h"
#include "Transform.h"

namespace Vanguard
{
	TYPE_DEFINITION(Entity, VanguardObject)

	Entity::Entity() : VanguardObject()
		, enabled(true)
		, tickEnabled (false)
		, tickRegistered (false)
	{
	}

	Entity::~Entity()
	{
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
			world->RegisterObject(newComponent);
		}
		components.PushBack(newComponent);
		componentTypeMap[aComponentType->GetRuntimeHash()] = newComponent;

		ComponentAdded(newComponent);

		return newComponent;
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