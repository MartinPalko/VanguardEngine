#include <stdio.h>

#include "gtest/gtest.h"

int main(int argc, char **argv)
{
	printf("Running main() from gtest_main.cc\n");
	testing::InitGoogleTest(&argc, argv);
	int result = RUN_ALL_TESTS();
	system("PAUSE");
	return result;
}
