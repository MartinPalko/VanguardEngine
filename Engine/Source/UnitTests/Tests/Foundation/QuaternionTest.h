#include "gtest/gtest.h"
#include "Types/Quaternion.h"

namespace Vanguard
{

	class QuaternionTest : public ::testing::Test
	{
	protected:
		virtual void SetUp()
		{

		}

		virtual void TearDown()
		{

		}
	};

	TEST_F(QuaternionTest, CheckNormalized)
	{
		Quaternion normalizedQuat(0.121696115f, 0.992291510f, 0.0f, 0.0234031007f);
		EXPECT_TRUE(normalizedQuat.IsNormalized());
		Quaternion notNormalizedQuat(5.2f, 42.4f, 0.0f, 1.0f);
		EXPECT_FALSE(notNormalizedQuat.IsNormalized());
	}

	TEST_F(QuaternionTest, Normalize)
	{
		Quaternion testQuat(5.2f, 42.4f, 0.0f, 1.0f);
		EXPECT_FALSE(testQuat.IsNormalized());
		testQuat.Normalize();
		EXPECT_TRUE(testQuat.IsNormalized());

	}


}
