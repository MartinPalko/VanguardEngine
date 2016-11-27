#include "RigidBody2D.h"

namespace Vanguard
{
	TYPE_DEFINITION(RigidBody2D, Component);

	RigidBody2D::RigidBody2D() : box2DBody(nullptr)
	{
	}

	RigidBody2D::~RigidBody2D()
	{
	}
}