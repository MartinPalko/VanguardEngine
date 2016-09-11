#include "SpriteRenderer.h"
#include "Entity/Actor.h"

namespace Vanguard
{
	TYPE_DEFINITION(SpriteRenderer, Component);

	void SpriteRenderer::SetDimensions(Vector2 aDimensions)
	{
		dimensions = aDimensions;
		if (GetEntity()->GetClassInfo()->IsA(Type::GetType<Actor>()))
			static_cast<Actor*>(GetEntity())->UpdateBounds();
	}

}