#include "gtest/gtest.h"

// Uncomment headers for which unit tests you want to run.
#include "StringTest.h"
#include "StringIDTest.h"
#include "FileSystemTest.h"
#include "DictionaryTest.h"

int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);
	int result = RUN_ALL_TESTS();
	system("PAUSE");
	return result;
}
