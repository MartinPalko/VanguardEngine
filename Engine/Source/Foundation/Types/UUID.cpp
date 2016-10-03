#include "UUID.h"

#ifdef WIN32
#include <Rpc.h>
#pragma comment(lib, "rpcrt4.lib")
#else
#include <cstring>
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
		static_assert(sizeof(uuid_t) == sizeof(data), "Size of UUID data is incorrect");
		uuid_generate_random (*(uuid_t*)newUUID.data);
#endif
		return newUUID;
	}

	UUID UUID::FromString(const String& aGUIDString)
	{		
		UUID newUUID;
#if defined(VANGUARD_WINDOWS)
		unsigned char* str = (unsigned char*)aGUIDString.GetCharPointer();
		UuidFromString(str, (::UUID*)newUUID.data);
#else
		uuid_parse(aGUIDString.GetCharPointer(), *(uuid_t*)newUUID.data);
#endif
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
		char str[37];
		uuid_unparse(*(uuid_t*)data, str);
		return str;
#endif
	}

	bool UUID::operator==(const UUID & aOther) const
	{
		return !memcmp(&data, &aOther.data, sizeof(data));
	}
}
