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
		// Compare a few values against a known answers to verify our implementation.
		const char* sampleData1 = "me7G72RP";
		const uint32 result1 = Crc32::CalcCrc(sampleData1, 8);
		EXPECT_EQ(0xAD1B25C9, result1);

		const char* sampleData2 = "sMpZcTfr";
		const uint32 result2 = Crc32::CalcCrc(sampleData2, 8);
		EXPECT_EQ(0xD42A85B8, result2);

		const char* sampleData3 = "e2eD2JmVG4vDH9KXDT7uEduD";
		const uint32 result3 = Crc32::CalcCrc(sampleData3, 24);
		EXPECT_EQ(0xC9B3AD6D, result3);

		EXPECT_NE(result1, result2);
		EXPECT_NE(result2, result3);
		EXPECT_NE(result3, result1);
	}

	TEST_F(UtilitiesTest, ConstExprCrc32)
	{
		uint32 result;

		// Compare a few values against a known answers to verify our implementation.
		const char* sampleData1 = "me7G72RP";
		result = Crc32::CalcCrc(sampleData1, 8);
		EXPECT_EQ(CRC32_STR("me7G72RP"), result);

		enum TestEnum
		{
			data2 = CRC32_STR("sMpZcTfr")
		};

		const char* sampleData2 = "sMpZcTfr";
		result = Crc32::CalcCrc(sampleData2, 8);
		EXPECT_EQ(TestEnum::data2, result);
	}
}