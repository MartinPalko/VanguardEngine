#include <iostream>
#include <vector>

#include "TypeInfo.h"
#include "Reflection.h"
#include "Foundation.h"
#include "Core.h"

void* Spawn(const char* aTypeName)
{
	TypeInfo* type = TypeInfo::GetType(aTypeName);
	if (type != NULL)
		return type->CreateInstance();
	else return NULL;
}

int main()
{
	//FoundationBaseClass* bC = new FoundationBaseClass();
	//FoundationDerivedClass* dC = new FoundationDerivedClass();

	std::vector<TypeInfo*>* allTypes = TypeInfo::GetAllTypes();

	std::cout << "Total Classes " << allTypes->size() << "\n\n";
	for (unsigned int i = 0; i < allTypes->size(); i++)
	{
		TypeInfo* type = allTypes->at(i);
		const char* typeName = type->GetTypeName();

		std::cout << "Class " << typeName << "\n";
	}

	//FoundationDerivedClass* bClassInst = (FoundationDerivedClass*)Spawn("BaseClass");
	//bClassInst->DoThing();

	system("pause");
}