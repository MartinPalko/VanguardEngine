#include "NativeClassInfo.h"

namespace Vanguard
{
	List<INativeClassInfo*> INativeClassInfo::allClassInfos = List<INativeClassInfo*>();

	List<INativeClassInfo*> INativeClassInfo::GetAllTypes()
	{
		return allClassInfos;
	}

	INativeClassInfo* INativeClassInfo::GetType(const String& aTypeName)
	{
		for (unsigned int i = 0; i < allClassInfos.Size(); i++)
		{
			if (allClassInfos[i]->GetTypeName() == aTypeName)
				return allClassInfos[i];
		}
		return nullptr;
	}
}