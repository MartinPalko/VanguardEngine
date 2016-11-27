#pragma once
#include "Physics2D_Common.h"

class b2Shape;

namespace Vanguard
{
	class PHYSICS2D_API Collider2D : public Component
	{
		friend class PhysicsWorld2D;
		ABSTRACT_TYPE_DECLARATION(Collider2D, Component)

	protected:
		virtual b2Shape* GetBox2DShape() = 0;
	};
}