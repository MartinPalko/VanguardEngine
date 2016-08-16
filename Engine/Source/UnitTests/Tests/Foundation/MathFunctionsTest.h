#include "gtest/gtest.h"
#include "Utility/MathFunctions.h"

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
			int32 iRandMin = Math::IRandom() / 2; // Div by 2, to keep the adding 1k from making the number go negative.
			int32 iRandMax = iRandMin + Math::IRandomRange(iRandMin, iRandMin + 1000);
			EXPECT_GE(iRandMax, iRandMin);
			int32 iRand = Math::IRandomRange(iRandMin, iRandMax);
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

	TEST_F(MathFunctionsTest, Pow)
	{
		EXPECT_EQ(1.42129948513f, Math::FPow(5.8f, 0.2f));
		EXPECT_EQ(128, Math::FPow(2, 7));
		EXPECT_EQ(0.0078125f, Math::FPow(2.0f, -7.0f));

		EXPECT_EQ(1, Math::IPow(5.8, 0.2));
		EXPECT_EQ(128, Math::IPow(2, 7));
		EXPECT_EQ(0, Math::IPow(2, -7));

		EXPECT_EQ(25, Math::Squared(5));
		EXPECT_EQ(125, Math::Cubed(5));

		EXPECT_EQ(9, Math::Squared(3));
		EXPECT_EQ(27, Math::Cubed(3));

		EXPECT_EQ(25.6036f, Math::Squared(5.06f));
		EXPECT_EQ(129.554216f, Math::Cubed(5.06f));
	}

	TEST_F(MathFunctionsTest, Trigonometry)
	{
		EXPECT_NEAR(-0.95892427466, Math::FSin(5), 0.000001);
		EXPECT_NEAR(0.0883686861, Math::FSin(135), 0.000001);

		EXPECT_NEAR(-0.84857027478, Math::FCos(34), 0.000001);
		EXPECT_NEAR(-0.91830665435, Math::FCos(-763), 0.000001);

		EXPECT_NEAR(-2.18503986326, Math::FTan(2), 0.000001);
		EXPECT_NEAR(0.08871575677, Math::FTan(-135), 0.000001);

		EXPECT_EQ(3.14159265359f, Math::Pi);
	}

	TEST_F(MathFunctionsTest, MaxMinAbs)
	{
		EXPECT_EQ(10.4654f, Math::Max(-5.546f, 10.4654f));
		EXPECT_EQ(11.4163f, Math::Max(10.9642f, 11.4163f));
		EXPECT_EQ(-75.5648f, Math::Max(-125.8784f, -75.5648f));

		EXPECT_EQ(10, Math::Max(10, -5));
		EXPECT_EQ(11, Math::Max(10, 11));
		EXPECT_EQ(-75, Math::Max(-125,-75));


		EXPECT_EQ(-5.546f, Math::Min(-5.546f, 10.4654f));
		EXPECT_EQ(10.9642f, Math::Min(10.9642f, 11.4163f));
		EXPECT_EQ(-125.8784f, Math::Min(-125.8784f, -75.5648f));

		EXPECT_EQ(-5, Math::Min(-5, 10));
		EXPECT_EQ(10, Math::Min(10, 11));
		EXPECT_EQ(-125, Math::Min(-125, -75));


		EXPECT_EQ(5.3f, Math::FAbs(-5.3f));
		EXPECT_EQ(0.0f, Math::FAbs(-0.0f));
		EXPECT_EQ(125.215f, Math::FAbs(125.215f));

		EXPECT_EQ(5, Math::FAbs(-5));
		EXPECT_EQ(0, Math::FAbs(-0));
		EXPECT_EQ(125, Math::FAbs(125));
	}
}
