#pragma once
#include "Renderer2D_Common.h"
#include "RenderableComponent2D.h"

struct SDL_Renderer;
struct SDL_Surface;
struct SDL_Texture;
struct _TTF_Font;
typedef struct _TTF_Font TTF_Font;

namespace Vanguard
{
	class RENDERER2D_API TextRenderer : public RenderableComponent2D
	{
	public:

		TYPE_DECLARATION(TextRenderer, RenderableComponent2D)
	private:
		String text;

		bool surfaceDirty;
		SDL_Surface* sdlSurface;
		std::unordered_map<SDL_Renderer*, SDL_Texture*> sdlTextures;
		std::unordered_map<SDL_Renderer*, bool> sdlTexturesDirty;
		TTF_Font* font;

	public:
		TextRenderer();
		~TextRenderer();

		void SetText(String aText);
		String GetText() { return text; }

	protected:
		virtual SDL_Texture* GetSDLTexture(SDL_Renderer* aRenderer) override;

	};
}