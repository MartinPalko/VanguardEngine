#include "gtest\gtest.h"

#include "FilePath.h"

namespace Vanguard
{
	class FilePathTest : public ::testing::Test
	{
	protected:
		virtual void SetUp()
		{

		}

		virtual void TearDown()
		{

		}
	};

	TEST_F(FilePathTest, FileCreationAndDeletion)
	{
		// Create a file on c drive called test.txt
		FilePath File = FilePath("C:/Test.txt");

		// Shouldn't exist initially
		EXPECT_FALSE(File.ExistsAsFile());

		// Make sure all permutations function as expected
		EXPECT_EQ("C:\\Test.txt", File.GetFullPathName());
		EXPECT_EQ("Test.txt", File.GetFilename());
		EXPECT_EQ("Test", File.GetFilenameWithoutExtension());
		EXPECT_EQ(".txt", File.GetFileExtension());

		// Create file on disk
		EXPECT_EQ("", File.CreateFileOnDisk());
		// Check it exists
		EXPECT_TRUE(File.ExistsAsFile());
		// Delete from disk
		EXPECT_TRUE(File.DeleteFromDisk());
		// Check it doesn't exist
		EXPECT_FALSE(File.ExistsAsFile());
	}

	TEST_F(FilePathTest, DirectoryCreationAndDeletion)
	{
		FilePath Directory = FilePath("C:/Test");

		EXPECT_FALSE(Directory.ExistsAsDirectory());

		EXPECT_EQ("", Directory.CreateDirectoryOnDisk());

		EXPECT_TRUE(Directory.ExistsAsDirectory());

		EXPECT_TRUE(Directory.DeleteFromDisk());

		EXPECT_FALSE(Directory.ExistsAsDirectory());
	}
}