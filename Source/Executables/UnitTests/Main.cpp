#include "gtest/gtest.h"

// Uncomment headers of unit tests you want to run.

// Foundation
#include "DictionaryTest.h"
#include "FileSystemTest.h"
#include "HashtableTest.h"
#include "MathFunctionsTest.h"
#include "StringIDTest.h"
#include "StringTest.h"
#include "VectorTest.h"

int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);
	int result = RUN_ALL_TESTS();
	system("PAUSE");
	return result;
}
