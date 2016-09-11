#include "SpriteRenderer.h"
#include "Brick.h"

namespace BrickBreaker
{
	TYPE_DEFINITION(Brick, Actor)

	Brick::Brick() : Actor()
	{
		SpriteRenderer* sprite = AddComponent<SpriteRenderer>();
		sprite->SetColor(Color(255, 120, 120, 255));
		sprite->SetDimensions(Vector2(6.0f, 2.0f));
		EnableTick();
	}

	void Brick::Tick(Frame* aFrame)
	{
		for (int i = 0; i < 100; i++)
		{
			Vector3 v (Math::FRandom(), Math::FRandom(), Math::FRandom());
			v = v.Normalize();
		}
	}
}