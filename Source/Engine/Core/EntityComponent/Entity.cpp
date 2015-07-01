#include "Entity.h"
#include "Component.h"

namespace Vanguard
{
	REFLECTED_SUBCLASS_DEFINITION(Entity,VanguardObject)

	Component* Entity::AddComponent(INativeClassInfo* aComponentType)
	{
		INativeClassInfo* componentType = INativeClassInfo::GetType("Component");

		bool isa = aComponentType->IsA(componentType);

		if (!isa)
		{
			DEBUG_WARN("Cannot add component. Class of type " + aComponentType->GetTypeName() + " does not derive from Component");
			return nullptr;
		}

		Component* newComponent = (Component*)aComponentType->CreateInstance();

		newComponent->entity = this;
		newComponent->world = GetWorld();
		return newComponent;
	}
}