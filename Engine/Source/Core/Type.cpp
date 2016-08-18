#include "Type.h"
#include "Log.h"

namespace Vanguard
{
	std::unordered_map<size_t, Type*>& Type::GetClassinfoNameMap()
	{
		static std::unordered_map<size_t, Type*> allClassInfos;
		return allClassInfos;
	}

	std::unordered_map<size_t, Type*>& Type::GetClassinfoHashMap()
	{
		static std::unordered_map<size_t, Type*> allClassInfos;
		return allClassInfos;
	}

	Type* Type::Register(IClassFactory * aClassFactory, size_t aRuntimeHash, const char * aClassName, const char * aBaseClassName)
	{
		// TODO: Create Unregister, and make sure it's called when DLL is unloaded.
		
		std::unordered_map<size_t, Type*>& nameMap = GetClassinfoNameMap();
		std::unordered_map<size_t, Type*>& hashMap = GetClassinfoHashMap();

		if (hashMap.count(aRuntimeHash))
		{
			// TODO: Throw an error when this happens (after unregistering is integrated)
			delete hashMap[aRuntimeHash];
			//return hashMap[aRuntimeHash];
		}

		Type* newClassInfo = new Type(aClassFactory, aClassName, aBaseClassName, aRuntimeHash);
		nameMap[StringID(aClassName).GetHash()] = newClassInfo;
		hashMap[aRuntimeHash] = newClassInfo;

		// TODO: Do this initially, and then again whenever a dll is loaded or unloaded, instead of every time a class is registered.
		UpdateHierarchyReferences();

		return newClassInfo;
	}

	DynamicArray<Type*> Type::GetAllTypes()
	{
		DynamicArray<Type*> returnArray(GetClassinfoNameMap().size());
		for (auto pair : GetClassinfoNameMap())
		{
			returnArray.PushBack(pair.second);
		}
		return returnArray;
	}

	Type* Type::GetType(const StringID& aTypeName)
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

	bool Type::IsA(Type* otherClass) const
	{
		const Type* currentClass = this;

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