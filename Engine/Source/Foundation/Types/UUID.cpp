#include "UUID.h"

#ifdef WIN32
#include <Rpc.h>
#pragma comment(lib, "rpcrt4.lib")
#else
#include <uuid/uuid.h>
#endif

namespace Vanguard
{
	UUID::UUID()
	{
#if defined(VANGUARD_WINDOWS)
		static_assert(sizeof(::UUID) == sizeof(data), "Size of UUID data is incorrect");
		CoCreateGuid((::UUID*)data);
#else
		// TODO
		static_assert(sizeof(uuid_t) == sizeof(data), "Size of UUID data is incorrect");
		uuid_t uuid;
		uuid_generate_random ( uuid );

#endif
	}

	UUID::UUID(const String& aGUIDString)
	{
		unsigned char* str = (unsigned char*)aGUIDString.GetCharPointer();
		UuidFromString(str, (::UUID*)data);
	}

	UUID::~UUID()
	{
	}

	String UUID::ToString()
	{
#if defined(VANGUARD_WINDOWS)
		unsigned char* str;
		UuidToString((::UUID*)data, &str );
		String uuidString((char*)str);
		RpcStringFree(&str);
		return uuidString;
#else
		// TODO
		//char s[37];
		//uuid_unparse ( uuid, s );
		return "";
#endif
	}

	bool UUID::operator==(const UUID & aOther) const
	{
		return !memcmp(&data, &aOther.data, sizeof(data));
	}
}
