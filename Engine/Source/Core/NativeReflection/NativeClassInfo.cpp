#include "NativeClassInfo.h"
#include "Log.h"

namespace Vanguard
{
	// TODO: Use a hashtable instead of an array!
	DynamicArray<INativeClassInfo*>& INativeClassInfo::GetAllClassInfosList()
	{
		static DynamicArray<INativeClassInfo*> allClassInfos;
		return allClassInfos;
	}

	DynamicArray<INativeClassInfo*> INativeClassInfo::GetAllTypes()
	{
		return GetAllClassInfosList();
	}

	INativeClassInfo* INativeClassInfo::GetType(const String& aTypeName)
	{
		DynamicArray<INativeClassInfo*>& allClassInfos = GetAllClassInfosList();

		for (unsigned int i = 0; i < allClassInfos.Count(); i++)
		{
			if (allClassInfos[i]->GetTypeName() == aTypeName)
				return allClassInfos[i];
		}
		return nullptr;
	}

	bool INativeClassInfo::IsA(INativeClassInfo* otherClass) const
	{
		const INativeClassInfo* currentClass = this;

		// Recurse up parents until we either find the class we're looking for (and return true) or reach a class with no base class (and return false)
		while (currentClass != nullptr)
		{
			if (currentClass->className == otherClass->className)
				return true;

			currentClass = currentClass->GetBaseClass();
		}
		return false;
	}
}