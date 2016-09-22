#pragma once
#include "Resources_Common.h"
#include "Resource.h"

namespace Vanguard
{
	class RESOURCES_API ImageResource : public Resource
	{
		ABSTRACT_TYPE_DECLARATION(ImageResource, Resource);

	public:
		virtual int GetStride() = 0;
		virtual int GetPixelDepth() = 0;
	};
}