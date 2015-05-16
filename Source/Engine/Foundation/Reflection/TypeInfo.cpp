#include "TypeInfo.h"

//std::vector<TypeInfo*> TypeInfo::allTypes;

void TypeInfo::AddToDatabase()
{
	GetAllTypes()->push_back(this);
}

std::vector<TypeInfo*>* TypeInfo::GetAllTypes()
{
	static std::vector<TypeInfo*>* allTypes = new std::vector<TypeInfo*>();
	return allTypes;
}

TypeInfo* TypeInfo::GetType(const char* aTypeName)
{
	std::vector<TypeInfo*>* allTypes = TypeInfo::GetAllTypes();
	for (unsigned int i = 0; i < allTypes->size(); i++)
	{
		if (strcmp(allTypes->at(i)->GetTypeName(), aTypeName))
			return allTypes->at(i);
	}
	return NULL;
}