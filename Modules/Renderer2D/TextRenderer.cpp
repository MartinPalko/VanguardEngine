#include "TextRenderer.h"
#include "WorldObjects/Actor.h"
#include "ResourceManager.h"
#include "VanguardSDL.h"

namespace Vanguard
{
	TYPE_DEFINITION(TextRenderer, RenderableComponent2D);

	TextRenderer::TextRenderer() : RenderableComponent2D()
		, text("")
		, surfaceDirty(true)
		, sdlSurface(nullptr)
		, sdlTextures()
	{
		SetBlendMode(eSpriteBlendMode::Blend);

		String fontPath = Core::GetInstance()->GetResourceManager()->FindAbsoluteResourcePath("Fonts/CourierCode-Roman.ttf");
		font = TTF_OpenFont(fontPath.GetCharPointer(), 110);
	}

	TextRenderer::~TextRenderer()
	{
		if (sdlSurface)
			SDL_FreeSurface(sdlSurface);

		for (auto item : sdlTextures)
			SDL_DestroyTexture(item.second);
	}

	void TextRenderer::SetText(String aText)
	{
		text = aText;
		surfaceDirty = true;
	}

	SDL_Texture* TextRenderer::GetSDLTexture(SDL_Renderer* aRenderer)
	{
		if (text == String())
			return nullptr;

		if (surfaceDirty)
		{
			if (sdlSurface)
				SDL_FreeSurface(sdlSurface);

			// Font surface is always white, and colored when rendering.
			sdlSurface = TTF_RenderText_Blended(font, text.GetCharPointer(), {255,255,255});
			//sdlSurface = TTF_RenderText_Solid(font, text.GetCharPointer(), {255,255,255});

			surfaceDirty = false;

			// Mark all textures dirty
			for (auto item : sdlTexturesDirty)
				sdlTexturesDirty[item.first] = true;
		}

		bool textureDirty = sdlTexturesDirty.count(aRenderer) ? sdlTexturesDirty.at(aRenderer) : true;
		SDL_Texture* texture = sdlTextures.count(aRenderer) ? sdlTextures.at(aRenderer) : nullptr;

		if (textureDirty)
		{
			if (texture)
				SDL_DestroyTexture(texture);

			texture = SDL_CreateTextureFromSurface(aRenderer, sdlSurface);
			sdlTextures[aRenderer] = texture;
			sdlTexturesDirty[aRenderer] = false;
		}

		return texture;
	}
}