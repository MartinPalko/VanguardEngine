#include "gtest/gtest.h"

// Comment out headers of unit tests you want to disable

// Foundation
#include "Tests/Foundation/FileSystemTest.h"
#include "Tests/Foundation/MathFunctionsTest.h"
#include "Tests/Foundation/StringIDTest.h"
#include "Tests/Foundation/StringTest.h"
#include "Tests/Foundation/VectorTest.h"
#include "Tests/Foundation/MatrixTest.h"
#include "Tests/Foundation/QuaternionTest.h"
#include "Tests/Foundation/UtilitiesTest.h"

// Core
#include "Tests/Core/EntityComponentTest.h"
#include "Tests/Core/NativeReflectionTest.h"
#include "Tests/Core/ConfigFileTest.h"

int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);
	int result = RUN_ALL_TESTS();
	return result;
}
