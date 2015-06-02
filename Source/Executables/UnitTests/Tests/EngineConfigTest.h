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
	};

	TEST_F(EngineConfigTest, GeneralTests)
	{
		//EXPECT_TRUE(EngineConfig::LoadConfigFromDisk());

		const Int32ConfigVar configBasedInt = Int32ConfigVar("ConfigBasedInt",50);

		//int32 intToSet = 4;
		//float floatToSet = 5.643f;
		//bool booleanToSet = false;
		//String stringToSet = "TestString";

		////EngineConfig::SetConfigInt("TestInt",intToSet);
		////EngineConfig::SetConfigFloat("TestFloat",floatToSet);
		////EngineConfig::SetConfigBoolean("TestBoolean", booleanToSet);
		////EngineConfig::SetConfigString("TestString", stringToSet);

		////EngineConfig::SaveConfigToDisk();

		//EngineConfig::LoadConfigFromDisk();

		//EXPECT_EQ(4, EngineConfig::GetConfigInt("TestInt",0));
		//EXPECT_EQ(5.643f, EngineConfig::GetConfigFloat("TestFloat",0.1f));
		//EXPECT_EQ(false, EngineConfig::GetConfigBoolean("TestBoolean",true));
		//EXPECT_EQ("TestString", EngineConfig::GetConfigString("TestString","NOT_THERE"));
	}
}