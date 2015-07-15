#include "StringID.h"

#include "zlib.h"

namespace Vanguard
{
	void StringID::HashString(const String& aString)
	{
		Hash = 0;
		Hash = crc32((uLong)Hash, (const Byte*)(aString.GetCharPointer()), (uLong)aString.GetLength());
	}
}