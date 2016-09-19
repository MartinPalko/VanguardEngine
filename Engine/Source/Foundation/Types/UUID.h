#pragma once
#include "Foundation_Common.h"
#include "VanguardString.h"

namespace Vanguard
{
	class FOUNDATION_API UUID
	{
		friend struct std::hash<Vanguard::UUID>;

		byte data[16];
	public:
		UUID();
		UUID(const String& aGUIDString);
		~UUID();

		String ToString();

		bool operator ==(const UUID& aOther) const;
	};
}

namespace std
{
	template <> 
	struct hash<Vanguard::UUID>
	{
		size_t operator()(Vanguard::UUID const& x) const noexcept
		{
			// Just use the first bit of data as our hash
			return *(size_t*)x.data;
		}
	};
}