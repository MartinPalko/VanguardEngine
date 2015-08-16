#include "Renderer2D_Common.h"

namespace Vanguard
{
	class RENDERER2D_API SpriteComponent : public Component
	{
		static int count;
	public:
		SpriteComponent()
		{
			count++;
			DEBUG_LOG(String::FromInt32(count))
		}
	};
}