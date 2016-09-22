#include "SDLImageResource.h"
#include "VanguardSDL.h"

namespace Vanguard
{
	TYPE_DEFINITION(SDLImageResource, ImageResource);

	bool SDLImageResource::LoadResource()
	{
		if (sdlSurface)
			return false;

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
			return true;
		}
		return false;
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