#include "Entity.h"
#include "Component.h"
#include "World.h"

namespace Vanguard
{
	REFLECTED_SUBCLASS_DEFINITION(Entity, VanguardObject)

	Component* Entity::AddComponent(NativeClassInfo* aComponentType)
	{
		static NativeClassInfo* componentType = NativeClassInfo::GetType("Component");

		bool isa = aComponentType->IsA(componentType);

		if (!isa)
		{
			DEBUG_WARN("Cannot add component. Class of type " + aComponentType->GetTypeName() + " does not derive from Component");
			return nullptr;
		}

		Component* newComponent = (Component*)aComponentType->CreateInstance();

		newComponent->entity = this;
		if (GetWorld())
		{
			newComponent->world = GetWorld();
			GetWorld()->objects.PushBack(newComponent);
		}
		return newComponent;
	}
}