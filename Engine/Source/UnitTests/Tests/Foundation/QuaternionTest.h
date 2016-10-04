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

	TEST_F(QuaternionTest, EulerAngles)
	{
		const float tolerance = 0.001f;

		for (int i = 0; i < 1000; i++)
		{
			Quaternion testQuat(Math::FRandomRange(-100, 100), Math::FRandomRange(-100, 100), Math::FRandomRange(-100, 100), Math::FRandomRange(-100, 100));
			testQuat.Normalize();

			EulerAngles euler = testQuat.ToEuler();
			Quaternion fromEuler = Quaternion::FromEuler(euler);
			EulerAngles eulerAgain = fromEuler.ToEuler();

			EXPECT_TRUE(Math::AboutEqual(euler.x, eulerAgain.x, tolerance));
			EXPECT_TRUE(Math::AboutEqual(euler.y, eulerAgain.y, tolerance));
			EXPECT_TRUE(Math::AboutEqual(euler.z, eulerAgain.z, tolerance));

			// TODO: Fails if all quaternion values are negative to the one they're comparing against (which is actually the same rotation)
			//EXPECT_TRUE(Math::AboutEqual(testQuat.x, fromEuler.x, tolerance));
			//EXPECT_TRUE(Math::AboutEqual(testQuat.y, fromEuler.y, tolerance));
			//EXPECT_TRUE(Math::AboutEqual(testQuat.z, fromEuler.z, tolerance));
			//EXPECT_TRUE(Math::AboutEqual(testQuat.w, fromEuler.w, tolerance));
		}
	}

}
