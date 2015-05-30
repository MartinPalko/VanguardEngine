#include "gtest\gtest.h"

#include "FileSystem.h"

namespace Vanguard
{
	class FileSystemTest : public ::testing::Test
	{
	protected:
		virtual void SetUp()
		{

		}

		virtual void TearDown()
		{

		}
	};

	TEST_F(FileSystemTest, DirectoryLocations)
	{
		FilePath engineRoot = FileSystem::GetEngineRootDirectory();
		FilePath configDir = FileSystem::GetEngineConfigDirectory();
		FilePath tempDir = FileSystem::GetEngineTempDirectory();

		// Print so they can be verified manually.
		printf("Engine Root: ");
		printf(engineRoot.GetFullPathName().c_str());
		printf("\n");

		printf("Config Directory: ");
		printf(configDir.GetFullPathName().c_str());
		printf("\n");

		printf("Engine Temp: ");
		printf(tempDir.GetFullPathName().c_str());
		printf("\n");

		EXPECT_TRUE(FileSystem::DirectoryExists(engineRoot + "Bin"));
		EXPECT_EQ(configDir, engineRoot.GetRelative("Config"));
		EXPECT_TRUE(FileSystem::DirectoryExists(tempDir));
	}

	TEST_F(FileSystemTest, FileCreationAndDeletion)
	{
		// Create a file on c drive called test.txt
		FilePath File = FilePath("C:/Test.txt");

		// Shouldn't exist initially
		EXPECT_FALSE(FileSystem::FileExists(File));

		// Make sure all permutations function as expected
		EXPECT_EQ("C:\\Test.txt", File.GetFullPathName());
		EXPECT_EQ("Test.txt", File.GetFilename());
		EXPECT_EQ("Test", File.GetFilenameWithoutExtension());
		EXPECT_EQ(".txt", File.GetFileExtension());

		// Create file on disk
		EXPECT_EQ("", FileSystem::CreateFile(File));
		// Check it exists
		EXPECT_TRUE(FileSystem::FileExists(File));
		// Delete from disk
		EXPECT_TRUE(FileSystem::Delete(File));
		// Check it doesn't exist
		EXPECT_FALSE(FileSystem::FileExists(File));

	}

	TEST_F(FileSystemTest, DirectoryCreationAndDeletion)
	{
		FilePath Directory = FilePath("C:/Test");

		EXPECT_FALSE(FileSystem::DirectoryExists(Directory));

		EXPECT_EQ("", FileSystem::CreateDirectory(Directory));

		EXPECT_TRUE(FileSystem::DirectoryExists(Directory));

		EXPECT_TRUE(FileSystem::Delete(Directory));

		EXPECT_FALSE(FileSystem::DirectoryExists(Directory));
	}
}