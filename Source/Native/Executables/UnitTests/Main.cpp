#include "gtest/gtest.h"

// Comment out headers of unit tests you want to disable

// Foundation
#include "DictionaryTest.h"
#include "FileSystemTest.h"
#include "HashtableTest.h"
#include "MathFunctionsTest.h"
#include "StringIDTest.h"
#include "StringTest.h"
#include "VectorTest.h"
#include "ConfigFileTest.h"

// Core
#include "EntityComponentTest.h"
#include "NativeReflectionTest.h"

int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);
	int result = RUN_ALL_TESTS();
	return result;
}
