#pragma once
#include "VanguardObject.h"
#include "Log.h"
#include "Component.h"

namespace Vanguard
{
	class Component;

	class CORE_API Entity : public VanguardObject
	{
		REFLECTED_SUBCLASS_DECLARATION(Entity,VanguardObject)

	private:
		DynamicArray<Component*> components;

		Entity* parent = nullptr;
		DynamicArray<Entity*> children;

	public:
		inline size_t GetNumComponents() { return components.Count(); }
		inline Component* GetComponent(size_t aIndex) { return components[aIndex]; }

		inline Entity* GetParent() { return parent; }
		inline bool HasParent() { return parent != nullptr; }
		
		inline size_t GetNumChildren() { return children.Count(); }
		inline Entity* GetChild(size_t aIndex) { return children[aIndex]; }

		Component* AddComponent(INativeClassInfo* aComponentType);
	};
}