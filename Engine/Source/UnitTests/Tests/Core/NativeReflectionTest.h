#include "gtest/gtest.h"
#include "Type.h"
#include "Log.h"

namespace Vanguard
{

	class NativeReflectionTest : public ::testing::Test
	{
	protected:
		virtual void SetUp()
		{

		}

		virtual void TearDown()
		{

		}

		String GetClassHierarchyString(Type* Class, int currentDepth, bool lastChild = false, String logmessage = "")
		{
			for (int i = 0; i < currentDepth; i++)
			{
				logmessage += " ";

				if (i == currentDepth - 1)
				{
					if (lastChild)
						logmessage += String((char)192); // Corner
					else
						logmessage += String((char)195);
					
				}
				else
				{
					logmessage += String((char)179); // Vertical bar
				}
			}			
			logmessage += " ";

			logmessage += Class->GetTypeName() + "\n";

			DynamicArray<Type*> children = Class->GetDerivedClasses();
			for (size_t i = 0; i < children.Count(); i++)
				logmessage = GetClassHierarchyString(children[i], currentDepth + 1, i == children.Count() - 1, logmessage);

			return logmessage;
		}
	};

	TEST_F(NativeReflectionTest, LogClassTree)
	{

		DynamicArray<Type*> allTypes = Type::GetAllTypes();

		DEBUG_LOG(String::FromInt(allTypes.Count()) + " types found:");
		for (uint32 i = 0; i < allTypes.Count(); i++)
		{
			if (allTypes[i]->GetBaseClass() == nullptr)
				DEBUG_LOG("\n" + GetClassHierarchyString(allTypes[i], 0));
		}
	}
}
