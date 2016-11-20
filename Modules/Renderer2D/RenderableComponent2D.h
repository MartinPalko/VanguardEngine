#pragma once
#include "Renderer2D_Common.h"
#include "SDLImageResource.h"

struct SDL_Texture;
struct SDL_Renderer;

namespace Vanguard
{
	namespace eSpriteBlendMode
	{
		enum Type
		{
			None = 0x00000000,
			Blend = 0x00000001,
			Add = 0x00000002,
			Mod = 0x00000004
		};
	}

	namespace eSpriteFlip
	{
		enum Type
		{
			None = 0x00000000,
			Horizontal = 0x00000001,
			Vertical = 0x00000002,
			Both = 0x00000003
		};
	}

	class RENDERER2D_API RenderableComponent2D : public Component
	{
		friend class Renderer2D;
		ABSTRACT_TYPE_DECLARATION(RenderableComponent2D, Component)

	private:
		Vector2 dimensions;
		Color color;
		eSpriteBlendMode::Type blendMode;
		eSpriteFlip::Type flipped;

	public:
		RenderableComponent2D();

		inline Vector2 GetDimensions() const { return dimensions; }
		void SetDimensions(Vector2 aDimensions);

		inline Color GetColor() const { return color; }
		void SetColor(Color aColor) { color = aColor; }

		void SetBlendMode(eSpriteBlendMode::Type aBlendMode) { blendMode = aBlendMode; }
		eSpriteBlendMode::Type GetBlendMode() { return blendMode; }

		void SetFlipped(eSpriteFlip::Type aFlip) { flipped = aFlip; }
		eSpriteFlip::Type GetFlipped() { return flipped; }

		// Override Component
		virtual Box GetBounds() override { return Box(Vector3(dimensions.x, dimensions.y, 0)); }

	protected:
		virtual SDL_Texture* GetSDLTexture(SDL_Renderer* aRenderer) = 0;
	};
}