#pragma once
#include "VanguardObject.h"
#include "Log.h"
#include "Component.h"

namespace Vanguard
{
	class Component;
	class World;

	class CORE_API Entity : public VanguardObject
	{
		TYPE_DECLARATION(Entity, VanguardObject)

	private:
		DynamicArray<Component*> components;

		Entity* parent = nullptr;
		DynamicArray<Entity*> children;

	public:
		inline size_t GetNumComponents() { return components.Count(); }
		inline Component* GetComponent(size_t aIndex) { return components[aIndex]; }
		
		Component* GetComponent(Type* aComponentType);
		inline Component* GetComponent(const StringID& aComponentType) { return GetComponent(Type::GetType(aComponentType)); }
		template <class T> T* GetComponent() { return (T*)GetComponent(Type::GetType<T>()); }

		inline Entity* GetParent() { return parent; }
		inline bool HasParent() { return parent != nullptr; }
		
		inline size_t GetNumChildren() { return children.Count(); }
		inline Entity* GetChild(size_t aIndex) { return children[aIndex]; }

		Component* AddComponent(Type* aComponentType);
		inline Component* AddComponent(const StringID& aComponentType) { return AddComponent(Type::GetType(aComponentType)); }		
		template<class T> T* AddComponent() { return (T*)AddComponent(Type::GetType<T>()); }
	};
}