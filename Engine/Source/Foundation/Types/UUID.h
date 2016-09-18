#pragma once
#include "Foundation_Common.h"
#include "VanguardString.h"

namespace Vanguard
{
	class FOUNDATION_API UUID
	{
		byte data[16];
	public:
		UUID();
		UUID(const String& aGUIDString);
		~UUID();

		String ToString();

		bool operator ==(const UUID& aOther) const;
	};
}