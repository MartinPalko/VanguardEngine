#pragma once

#include "Renderer2D_Common.h"
#include "ImageResource.h"

struct SDL_Surface;
struct SDL_Texture;
struct SDL_Renderer;

namespace Vanguard
{
	class SDLImageResource : public ImageResource
	{
		friend class Renderer2D;

		TYPE_DECLARATION(SDLImageResource, ImageResource);

	private:
		SDL_Surface* sdlSurface;
		std::unordered_map<SDL_Renderer*, SDL_Texture*> sdlTextures; 

	public:
		SDLImageResource() : ImageResource()
			, sdlSurface(nullptr)
		{

		}

		~SDLImageResource() {}

		SDL_Texture* getSDLTexture(SDL_Renderer* sdlRenderer);

		// Implement Resource
		virtual bool LoadResource() override;
		virtual bool UnloadResource() override;
		virtual bool IsLoaded() override;
		virtual size_t GetSize() override;

		// Implement ImageResource
		virtual int GetStride() override;
		virtual int GetPixelDepth() override;
	};
}