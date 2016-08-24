#include "Brick.h"
#include "SpriteComponent.h"
namespace BrickBreaker
{
	TYPE_DEFINITION(Brick, Actor)

	Brick::Brick() : Actor()
	{
		SpriteComponent* sprite = AddComponent<SpriteComponent>();
		sprite->SetColor(Color(255, 120, 120, 255));
		sprite->SetDimensions(Vector2(6.0f, 2.0f));
	}
}