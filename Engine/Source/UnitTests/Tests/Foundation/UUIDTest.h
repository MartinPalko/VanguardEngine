#include "gtest/gtest.h"
#include "Types/UUID.h"

namespace Vanguard
{

	class UUIDTest : public ::testing::Test
	{
	protected:
		virtual void SetUp()
		{

		}

		virtual void TearDown()
		{

		}
	};

	TEST_F(UUIDTest, ToAndFromString)
	{
		UUID uuid;
		String str = uuid.ToString();
		UUID fromString(str);
		EXPECT_EQ(uuid, fromString);
	}
}
