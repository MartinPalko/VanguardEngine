#pragma once
#include "FoundationBaseClass.h"

#include "Reflection.h"

V_REFLECTED_CLASS(FoundationDerivedClass)

class DllExport FoundationDerivedClass : public FoundationBaseClass
{
public:
	FoundationDerivedClass();

	virtual void DoThing()
	{
		//std::cout << "\nI'M A " << GetType()->GetTypeName() << "DOING A THING\n";
	}
};