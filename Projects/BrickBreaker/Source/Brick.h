#pragma once
#include "BrickBreaker_Common.h"

namespace BrickBreaker
{
	class Brick : public Actor
	{
		TYPE_DECLARATION(Brick, Actor)
	public:
		Brick();

		virtual void Tick(Frame* aFrame) override;

	};
}