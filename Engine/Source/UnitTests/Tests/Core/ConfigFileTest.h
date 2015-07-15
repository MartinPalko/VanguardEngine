#include "gtest/gtest.h"

#include "Types/FilePath.h"
#include "Config/ConfigFile.h"

namespace Vanguard
{

	class ConfigFileTest : public ::testing::Test
	{
	protected:
		virtual void SetUp()
		{

		}

		virtual void TearDown()
		{

		}
	};

	TEST_F(ConfigFileTest, Loading)
	{
		FilePath configFilePath = FileSystem::GetSystemTempDirectory().GetRelative("Temp.cfg");

		String configFileContents;
		configFileContents += "[Section1]\n";
		configFileContents += "Value1=5;InlineComent\n";
		configFileContents += "Value2=Test\n";
		configFileContents += ";FullLineComment\n";
		configFileContents += "MultiValue=Element1\n";
		configFileContents += "MultiValue+=Element2\n";

		configFileContents += "#FullLineComment2\n";
		configFileContents += "[Section2]\n";
		configFileContents += "Value1=5#InlineComent2\n";
		configFileContents += "Value2=Test\n";
		configFileContents += "Value2-=Test\n"; // "Value2" should be deleted.
		configFileContents += "MultiValue+=Element1\n";
		configFileContents += "MultiValue+=Element2\n";
		configFileContents += "MultiValue+=Element3\n";


		FileSystem::WriteToFile(configFilePath, configFileContents);

		ConfigFile configFile = ConfigFile::Load(configFilePath);

		EXPECT_EQ(configFile.GetValue("Section1", "Value1"),"5");
		EXPECT_EQ(configFile.GetValue("Section2", "MultiValue"), "Element1,Element2,Element3");

		EXPECT_FALSE(configFile.ContainsValue("Section2", "Value2"));

		FileSystem::Delete(configFilePath);
	}
}
