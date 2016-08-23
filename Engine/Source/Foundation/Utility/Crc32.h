#pragma once
#include "Foundation_Common.h"

namespace Vanguard
{
	namespace Crc32
	{
		uint32 FOUNDATION_API CalcCrc(const byte* aData, size_t aDataLength);		
	}
}