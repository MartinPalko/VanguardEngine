#include "NativeClassInfo.h"
#include "Log.h"

namespace Vanguard
{
	std::unordered_map<size_t, INativeClassInfo*>& INativeClassInfo::GetClassinfoNameMap()
	{
		static std::unordered_map<size_t, INativeClassInfo*> allClassInfos;
		return allClassInfos;
	}

	std::unordered_map<size_t, INativeClassInfo*>& INativeClassInfo::GetClassinfoHashMap()
	{
		static std::unordered_map<size_t, INativeClassInfo*> allClassInfos;
		return allClassInfos;
	}

	DynamicArray<INativeClassInfo*> INativeClassInfo::GetAllTypes()
	{
		DynamicArray<INativeClassInfo*> returnArray(GetClassinfoNameMap().size());
		for (auto pair : GetClassinfoNameMap())
		{
			returnArray.PushBack(pair.second);
		}
		return returnArray;
	}

	INativeClassInfo* INativeClassInfo::GetType(const StringID& aTypeName)
	{
		const size_t nameHash = aTypeName.GetHash();
		if (GetClassinfoNameMap().count(nameHash))
		{
			return GetClassinfoNameMap()[nameHash];
		}
		else
		{
			return nullptr;
		}
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