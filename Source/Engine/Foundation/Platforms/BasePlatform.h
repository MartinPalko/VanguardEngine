#pragma once

#include <string>
namespace Vanguard
{
	struct BasePlatform
	{
		typedef ::std::string string; // Use an std string as our string class.

		typedef __int64 int64;
		typedef __int32 int32;
		typedef __int16 int16;

		typedef unsigned __int64 uint64;
		typedef unsigned __int32 uint32;
		typedef unsigned __int16 uint16;
	};
}