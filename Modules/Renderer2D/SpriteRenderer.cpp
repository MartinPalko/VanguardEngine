#include "SpriteRenderer.h"
#include "WorldObjects/Actor.h"
#include "ResourceManager.h"

namespace Vanguard
{
	TYPE_DEFINITION(SpriteRenderer, RenderableComponent2D);

	START_REGISTER_PROPERTIES(SpriteRenderer);
		REGISTER_PROPERTY(SpriteRenderer, image);
	FINISH_REGISTER_PROPERTIES(SpriteRenderer);

	void SpriteRenderer::SetImage(const String & aImageName)
	{
		image = Core::GetInstance()->GetResourceManager()->LoadResource<SDLImageResource>(aImageName);
	}

	SDL_Texture* SpriteRenderer::GetSDLTexture(SDL_Renderer* aRenderer)
	{
		return image ? image->getSDLTexture(aRenderer) : nullptr;
	}
}