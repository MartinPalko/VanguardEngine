#include "NativeClassInfo.h"
#include "Log.h"

namespace Vanguard
{
	//List<INativeClassInfo*> INativeClassInfo::allClassInfos = List<INativeClassInfo*>();
	List<INativeClassInfo*>& INativeClassInfo::GetAllClassInfosList()
	{
		static List<INativeClassInfo*> allClassInfos;
		return allClassInfos;
	}

	List<INativeClassInfo*> INativeClassInfo::GetAllTypes()
	{
		return GetAllClassInfosList();
	}

	INativeClassInfo* INativeClassInfo::GetType(const String& aTypeName)
	{
		List<INativeClassInfo*>& allClassInfos = GetAllClassInfosList();

		for (unsigned int i = 0; i < allClassInfos.Size(); i++)
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