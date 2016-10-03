#include "UUID.h"

#ifdef WIN32
#include <Rpc.h>
#pragma comment(lib, "rpcrt4.lib")
#else
#include <uuid/uuid.h>
#endif

namespace Vanguard
{
	UUID::UUID() : data{0}
	{
		
	}

	UUID::UUID(const UUID& aOther)
	{
		memcpy(data, aOther.data, sizeof(data));
	}

	UUID::~UUID()
	{
	}

	UUID UUID::CreateNew()
	{
		UUID newUUID;
#if defined(VANGUARD_WINDOWS)
		static_assert(sizeof(::UUID) == sizeof(data), "Size of UUID data is incorrect");
		CoCreateGuid((::UUID*)newUUID.data);
#else
		// TODO
		static_assert(sizeof(uuid_t) == sizeof(data), "Size of UUID data is incorrect");
		uuid_t uuid;
		uuid_generate_random ( uuid );
#endif
		return newUUID;
	}

	UUID UUID::FromString(const String& aGUIDString)
	{
		UUID newUUID;
		unsigned char* str = (unsigned char*)aGUIDString.GetCharPointer();
		UuidFromString(str, (::UUID*)newUUID.data);
		return newUUID;
	}

	UUID UUID::Null()
	{
		return UUID();
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
