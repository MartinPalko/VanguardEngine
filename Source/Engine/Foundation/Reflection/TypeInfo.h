#pragma once
#include <iostream>
#include <vector>

class TypeInfo
{
protected:
	static std::vector<TypeInfo*>* allTypes;
	void AddToDatabase();
public:
	virtual const char* GetTypeName() = 0;
	virtual void* CreateInstance() = 0;

	static std::vector<TypeInfo*>* GetAllTypes();
	static TypeInfo* GetType(const char* aTypeName);
};

template <class T> class TTypeInfo : public TypeInfo
{
public:
	const char* typeName;

	TTypeInfo(const char* aClassName)
	{
		typeName = aClassName;
		AddToDatabase();

		std::cout << "Constructed Type " << aClassName << "\n";
	}

	virtual const char* GetTypeName() override
	{
		return typeName;
	};

	virtual void* CreateInstance() override
	{
		return new T();
	};
};


