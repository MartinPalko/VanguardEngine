#include "gtest/gtest.h"
#include "Types/VanguardString.h"
#include "Types/StringID.h"
#include "Collections/Hashtable.h"

namespace Vanguard
{

	class HashtableTest : public ::testing::Test
	{
	protected:
		virtual void SetUp()
		{

		}

		virtual void TearDown()
		{

		}
	};

	TEST_F(HashtableTest, AddingElement)
	{
		Vanguard::Hashtable<String> testData;

		//testData[StringID("one")] = 1;
		testData[1] = "IntOne";
		EXPECT_EQ(testData[1], "IntOne");

		StringID id1 = "StringID";
		StringID id2 = "StringID";

		testData[StringID("StringID")] = "StringID";
		EXPECT_EQ(testData[StringID("StringID")], "StringID");
		EXPECT_EQ(testData["StringID"], "StringID");


		testData["HashedString"] = "String";
		EXPECT_EQ(testData["HashedString"], "String");
		EXPECT_EQ(testData[StringID("HashedString")], "String");
	}
}
