#include "gtest/gtest.h"
#include "StringID.h"

namespace Vanguard
{

	class StringIDTest : public ::testing::Test
	{
	protected:
		virtual void SetUp()
		{

		}

		virtual void TearDown()
		{

		}
	};

	TEST_F(StringIDTest, HashGeneration)
	{
		StringID testID = String("Test");
		StringID testIDIdentical = "Test";
		StringID testIDDifferent = StringID("Test2");

		EXPECT_EQ(testID, testIDIdentical);
		EXPECT_NE(testID, testIDDifferent);

	}
}
