#include "gtest/gtest.h"
#include "MathFunctions.h"

namespace Vanguard
{

	class MathFunctionsTest : public ::testing::Test
	{
	protected:
		virtual void SetUp()
		{

		}

		virtual void TearDown()
		{

		}
	};

	TEST_F(MathFunctionsTest, Random)
	{
		for (int i = 0; i < 10; i++)
		{
			int32 iRandMin = Math::Random();
			int32 iRandMax = iRandMin + Math::RandomRange(iRandMin, iRandMin + 1000);
			EXPECT_GE(iRandMax, iRandMin);
			int32 iRand = Math::RandomRange(iRandMin, iRandMax);
			//std::cout << "Min : " << iRandMin << " Max: " << iRandMax << " Value: " << iRand << "\n";
			EXPECT_LE(iRand, iRandMax);
			EXPECT_GE(iRand, iRandMin);
		}

		for (int i = 0; i < 10; i++)
		{
			float fRandMin = Math::FRandom();
			float fRandMax = fRandMin + Math::FRandomRange(fRandMin, fRandMin + 1000.0f);
			EXPECT_GE(fRandMax, fRandMin);
			float fRand = Math::FRandomRange(fRandMin, fRandMax);
			//std::cout << "Min : " << fRandMin << " Max: " << fRandMax << " Value: " << fRand << "\n";
			EXPECT_LE(fRand, fRandMax);
			EXPECT_GE(fRand, fRandMin);
		}

		for (int i = 0; i < 10; i++)
		{
			float fRand = Math::FRandom();
			//std::cout << " Value: " << fRand << "\n";
			EXPECT_LE(fRand, 1);
			EXPECT_GE(fRand, 0);
		}
	}

	TEST_F(MathFunctionsTest, Trigonometry)
	{
		EXPECT_NEAR(-0.95892427466, Math::Sin(5), 0.000001);
		EXPECT_NEAR(0.0883686861, Math::Sin(135), 0.000001);

		EXPECT_NEAR(-0.84857027478, Math::Cos(34), 0.000001);
		EXPECT_NEAR(-0.91830665435, Math::Cos(-763), 0.000001);

		EXPECT_NEAR(-2.18503986326, Math::Tan(2), 0.000001);
		EXPECT_NEAR(0.08871575677, Math::Tan(-135), 0.000001);

		EXPECT_EQ(3.14159265359f, Math::Pi);
	}

	TEST_F(MathFunctionsTest, FastInvSquareRoot)
	{
		//// Run fast inverse square root a million times, to make sure it falls within a reasonable tolerance.
		//for (int i = 0; i < 1000000; i++)
		//{
		//	float testValue = Math::FRandomRange(0.0001f, 10000000.0f);
		//	EXPECT_NEAR(1.0f / Math::Sqrt(testValue), Math::FastInvSqrt(testValue), 0.001f);
		//}
	}
}
