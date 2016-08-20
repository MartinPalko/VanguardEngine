#include "Renderer2D_Common.h"

namespace Vanguard
{
	class RENDERER2D_API SpriteComponent : public Component
	{
		TYPE_DECLARATION(SpriteComponent, Component)
	private:
		Vector2 dimensions;
		Color color;

	public:
		SpriteComponent() : Component()
			, dimensions(1, 1)
			, color(1, 1, 1, 1)
		{
		}

		inline Vector2 GetDimensions() const { return dimensions; }
		void SetDimensions(Vector2 aDimensions) { dimensions = aDimensions; }
		
		inline Color GetColor() const { return color; }
		void SetColor(Color aColor) { color = aColor; }

	};
}