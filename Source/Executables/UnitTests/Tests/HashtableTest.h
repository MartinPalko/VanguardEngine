#include "gtest/gtest.h"
#include "VanguardString.h"
#include "VanguardHashtable.h"

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
		Vanguard::Hashtable<std::string, int> testData;

		testData["one"] = 1;
		EXPECT_TRUE(testData.ContainsValue(1));
	}
}
