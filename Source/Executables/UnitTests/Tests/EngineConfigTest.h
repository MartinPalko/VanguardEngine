#include "gtest\gtest.h"

#include "Config.h"

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

		int32_Config(TestInt,"ConfigTest", 10);
		float_Config(TestFloat, "ConfigTest", 3.1415f);
		bool_Config(TestBool, "ConfigTest", true);
		String_Config(TestString, "ConfigTest", "Test");
	};

	class OtherTestClass
	{
		int32_Config(TestInt, "ConfigTest", 10);
		float_Config(TestFloat, "ConfigTest", 3.1415f);
		bool_Config(TestBool, "ConfigTest", true);
		String_Config(TestString, "ConfigTest", "Test");
		int32_Config(OtherTestInt, "ConfigTest", 10);
		float_Config(OtherTestFloat, "ConfigTest", 3.1415f);
		bool_Config(OtherTestBool, "ConfigTest", true);
		String_Config(OtherTestString, "ConfigTest", "Test");
	};

	TEST_F(EngineConfigTest, GeneralTests)
	{
		OtherTestClass otherClass = OtherTestClass();

		ConfigTable::LoadConfigFromDisk();

		EXPECT_EQ(10, TestInt);
		EXPECT_EQ(3.1415f, TestFloat);
		EXPECT_EQ(true, TestBool);
		EXPECT_EQ("Test", TestString);

		ConfigTable::SaveConfigToDisk();
	}
}