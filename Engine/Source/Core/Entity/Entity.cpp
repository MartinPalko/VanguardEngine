#include "Entity.h"
#include "Component.h"
#include "World.h"

namespace Vanguard
{
	TYPE_DEFINITION(Entity, VanguardObject)

	Component* Entity::GetComponent(Type* aComponentType)
	{
		Type* componentType = Type::GetType<Component>();

		if (!aComponentType->IsA(componentType))
		{
			Log::Warning("Cannot add component. Class of type " + aComponentType->GetTypeName() + " does not derive from Component");
			return nullptr;
		}

		// TODO: Store components in a hashmap by type.
		for (size_t i = 0; i < GetNumComponents(); i++)
		{
			if (components[i]->GetClassInfo()->IsA(aComponentType))
				return (Component*)components[i];
		}

		return nullptr;
	}

	Component* Entity::AddComponent(Type* aComponentType)
	{
		Type* componentType = Type::GetType<Component>();

		if (!aComponentType->IsA(componentType))
		{
			Log::Warning("Cannot add component. Class of type " + aComponentType->GetTypeName() + " does not derive from Component");
			return nullptr;
		}

		Component* newComponent = (Component*)aComponentType->CreateInstance();

		newComponent->entity = this;
		if (GetWorld())
		{
			newComponent->world = GetWorld();
			GetWorld()->objects.PushBack(newComponent);
		}
		components.PushBack(newComponent);

		return newComponent;
	}
}