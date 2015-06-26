#include "NativeClassInfo.h"
#include "Log.h"

namespace Vanguard
{
	//List<INativeClassInfo*> allTypes = INativeClassInfo::GetAllTypes();
	//Log::Write(allTypes.Size() + " types found:");
	//for (uint32 i = 0; i < allTypes.Size(); i++)
	//{
	//	if (allTypes[i]->GetBaseClass() == nullptr)
	//		PrintChildrenRecursively(allTypes[i], 0);
	//}

	void PrintChildrenRecursively(INativeClassInfo* Class, int currentDepth)
	{
		String logmessage;
		for (int i = 0; i < currentDepth; i++)
		{
			logmessage += "\t";
		}
		logmessage += Class->GetTypeName();
		Log::Write(logmessage);

		List<INativeClassInfo*> children = Class->GetDerivedClasses();
		for (size_t i = 0; i < children.Size(); i++)
			PrintChildrenRecursively(children[i], currentDepth + 1);
	}

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