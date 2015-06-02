#include "gtest\gtest.h"

#include "EngineConfig.h"
#include "ConfigVar.h"

namespace Vanguard
{
	class EngineConfigTest : public ::testing::Test
	{
	protected:
		virtual void SetUp()
		{

		}

		virtual void TearDown()
		{

		}

		int32_Config(TestInt, 10);
		float_Config(TestFloat, 3.1415f);
		bool_Config(TestBool, true);
		String_Config(TestString, "Test");
	};

	TEST_F(EngineConfigTest, GeneralTests)
	{
		EXPECT_TRUE(EngineConfig::LoadConfigFromDisk());

		EXPECT_EQ(10, TestInt);
		EXPECT_EQ(3.1415f, TestFloat);
		EXPECT_EQ(true, TestBool);
		EXPECT_EQ("Test", TestString);

		EngineConfig::SaveConfigToDisk();
	}
}