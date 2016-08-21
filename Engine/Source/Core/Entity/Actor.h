#pragma once
#include "Entity.h"
#include "Transform.h"
#include "World.h"

namespace Vanguard
{
	class CORE_API Actor : public Entity
	{
		friend World;
		TYPE_DECLARATION(Actor, Entity)
	private:
		TickFunction tickFunction;
		bool tickEnabled;
		bool tickRegistered;
		Transform* transform;
	public:
		Actor();
		
		void EnableTick();
		void DisableTick();

		Transform* GetTransform() { return transform; }

		virtual void Tick(Frame* aFrame) {}

	};
}