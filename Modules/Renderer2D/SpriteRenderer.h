#pragma once
#include "Renderer2D_Common.h"
#include "RenderableComponent2D.h"
#include "SDLImageResource.h"

namespace Vanguard
{
	class RENDERER2D_API SpriteRenderer : public RenderableComponent2D
	{
	public:

		TYPE_DECLARATION(SpriteRenderer, RenderableComponent2D)
	private:
		SDLImageResource* image;


	public:
		SpriteRenderer() : RenderableComponent2D()
			, image(nullptr)
		{
		}

		void SetImage(const String& aImageName);
		SDLImageResource* GetImage() { return image; }

	protected:
		virtual SDL_Texture* GetSDLTexture(SDL_Renderer* aRenderer) override;

	};
}