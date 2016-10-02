#include "SpriteRenderer.h"
#include "WorldObjects/Actor.h"
#include "ResourceManager.h"

namespace Vanguard
{
	TYPE_DEFINITION(SpriteRenderer, Component);

	void SpriteRenderer::SetDimensions(Vector2 aDimensions)
	{
		dimensions = aDimensions;
		if (GetEntity()->GetType()->IsA(Type::GetType<Actor>()))
			static_cast<Actor*>(GetEntity())->UpdateBounds();
	}

	void SpriteRenderer::SetImage(const String & aImageName)
	{
		image = Core::GetInstance()->GetResourceManager()->LoadResource<SDLImageResource>(aImageName);
	}
}