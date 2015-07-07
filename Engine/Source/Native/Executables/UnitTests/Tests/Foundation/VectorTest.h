#include "gtest/gtest.h"
#include "Vector3.h"
#include "Vector2.h"

namespace Vanguard
{

	class VectorTest : public ::testing::Test
	{
	protected:
		virtual void SetUp()
		{

		}

		virtual void TearDown()
		{

		}
	};

	TEST_F(VectorTest, Normalize)
	{
		// Ensure fast normalize gives reasonably close results
		for (int i = 0; i < 10000; i++)
		{
			Vector3 v3 = Vector3(Math::FRandomRange(-100, 100), Math::FRandomRange(-100, 100), Math::FRandomRange(-100, 100));
			EXPECT_NEAR(1, v3.Normalize().Length(), 0.000001);
		}
		for (int i = 0; i < 10000; i++)
		{
			Vector2 v2 = Vector2(Math::FRandomRange(-100, 100), Math::FRandomRange(-100, 100));
			EXPECT_NEAR(1, v2.Normalize().Length(), 0.000001);
		}
	}
}
