#include "SDLImageResource.h"
#include "VanguardSDL.h"

namespace Vanguard
{
	TYPE_DEFINITION(SDLImageResource, ImageResource);

	SDL_Texture* SDLImageResource::getSDLTexture(SDL_Renderer* sdlRenderer)
	{
		if (sdlTextures.count(sdlRenderer))
			return sdlTextures[sdlRenderer];

		if (!sdlSurface)
			return nullptr;

		SDL_Texture* texture = SDL_CreateTextureFromSurface(sdlRenderer, sdlSurface);
		sdlTextures[sdlRenderer] = texture;
		return texture;
	}

	bool SDLImageResource::LoadResource()
	{
		const String path = GetResourcePath();
		sdlSurface = IMG_Load(path.GetCharPointer());

		return sdlSurface;
	}

	bool SDLImageResource::UnloadResource()
	{
		if (sdlSurface)
		{
			SDL_FreeSurface(sdlSurface);
			sdlSurface = nullptr;
		}

		for (auto pair : sdlTextures)
		{
			SDL_DestroyTexture(pair.second);
		}
		sdlTextures.clear();

		return true;
	}

	bool SDLImageResource::IsLoaded()
	{
		return sdlSurface != nullptr;
	}

	size_t SDLImageResource::GetSize()
	{
		return sdlSurface->pitch * sdlSurface->h;
	}

	int SDLImageResource::GetStride()
	{
		return sdlSurface->pitch;
	}

	int SDLImageResource::GetPixelDepth()
	{
		return sdlSurface->format->BytesPerPixel;
	}
}