#include "gtest/gtest.h"

#include "Utility/Crc32.h"

namespace Vanguard
{
	class UtilitiesTest : public ::testing::Test
	{
	protected:
		virtual void SetUp()
		{

		}

		virtual void TearDown()
		{

		}
	};

	TEST_F(UtilitiesTest, Crc32)
	{
		char* sampleData;
		uint32 result;

		// Compare a few values against a known answers to verify our implementation.
		sampleData = "me7G72RP";
		result = Crc32::CalcCrc(sampleData, 8);
		EXPECT_EQ(0xAD1B25C9, result);

		sampleData = "sMpZcTfr";
		result = Crc32::CalcCrc(sampleData, 8);
		EXPECT_EQ(0xD42A85B8, result);

		sampleData = "e2eD2JmVG4vDH9KXDT7uEduD";
		result = Crc32::CalcCrc(sampleData, 24);
		EXPECT_EQ(0xC9B3AD6D, result);
	}
}