#include "gtest/gtest.h"
#include "Types/Matrix4x4.h"

namespace Vanguard
{

	class MatrixTest : public ::testing::Test
	{
	protected:
		virtual void SetUp()
		{

		}

		virtual void TearDown()
		{

		}
	};

	TEST_F(MatrixTest, Constructors)
	{
		float values1d[] =
		{
			1.0f, 2.0f, 3.0f, 4.0f,
			5.0f, 6.0f, 7.0f, 8.0f,
			9.0f, 10.0f, 11.0f, 12.0f,
			13.0f, 14.0f, 15.0f, 16.0f
		};

		float values2d[4][4] =
		{
			{1.0f, 2.0f, 3.0f, 4.0f},
		{5.0f, 6.0f, 7.0f, 8.0f},
		{9.0f, 10.0f, 11.0f, 12.0f},
		{13.0f, 14.0f, 15.0f, 16.0f}
		};
		Matrix4x4 from1d (values1d);
		Matrix4x4 from2d (values2d);
		EXPECT_EQ(from1d, from2d);
	}
}
