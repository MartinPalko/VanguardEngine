#pragma once
#include "Entity.h"
#include "Transform.h"

namespace Vanguard
{
	class CORE_API Actor : public Entity
	{
		TYPE_DECLARATION(Actor, Entity)
	private:
		Transform* transform;
		Box localBounds;
	public:
		Actor();	

		Transform* GetTransform() { return transform; }

		void UpdateBounds();
		Box GetLocalBounds() { return localBounds; }

		// From Entity
		virtual void ComponentAdded(Component* aComponent) override;
	};
}