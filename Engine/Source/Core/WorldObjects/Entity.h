#pragma once
#include "WorldObject.h"
#include "Log.h"
#include "Component.h"
#include "World.h"
#include "Jobs/Frame.h"

namespace Vanguard
{
	class Component;
	class World;
	class Transform;

	class CORE_API Entity : public WorldObject, public IWorldObjectEventListener
	{
		friend World;

		TYPE_DECLARATION(Entity, WorldObject)

	private:
		DynamicArray<Component*> components;
		std::map<size_t, Component*> componentTypeMap;

		Entity* parent = nullptr;
		DynamicArray<Entity*> children;
		bool enabled;

		bool tickEnabled;
		bool tickRegistered;

	public:
		Entity();
		~Entity();

		//Override WorldObject
		virtual void Destroy() override;

		// Implement IWorldObjectEventListener
		virtual void OnWorldObjectEvent(WorldObjectEvent* aEvent) override;

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
		virtual void ComponentAdded(Component* aComponent) {}

		void Enable() { enabled = true; }
		void Disable() { enabled = false; }
		bool Enabled() { return enabled; }

		void EnableTick();
		void DisableTick();
		virtual void Tick(Frame* aFrame) {}
		bool TickEnabled() { return tickEnabled; }
	};
}