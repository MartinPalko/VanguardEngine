#include "gtest\gtest.h"
#include "VanguardString.h"

namespace Vanguard
{

	class StringTest : public ::testing::Test
	{
	protected:
		virtual void SetUp()
		{

		}

		virtual void TearDown()
		{

		}
	};

	TEST_F(StringTest, CastsAndComparisons)
	{
		string vanguardFromLiteral = "test";
		string vanguardFromLiteral2 = "test";

		std::string stdFromVanguard = vanguardFromLiteral;
		std::string stdFromLiteral = "test";

		juce::String juceFromVanguard = vanguardFromLiteral;
		juce::String juceFromLiteral = "test";

		const char* charPointerFromVanguard = vanguardFromLiteral;
		const char* charPointerFromLiteral = "test";

		string vanguardFromStd = stdFromLiteral;
		string vanguardFromJuce = juceFromLiteral;
		string vanguardFromCharPointer = charPointerFromLiteral;
		
		// Verify all types have their values set properly, and can be compared interchangeably with vanguard string

		EXPECT_EQ(vanguardFromLiteral, vanguardFromLiteral2); // Vanguard String to Vanguard string
		EXPECT_EQ(vanguardFromLiteral, vanguardFromLiteral); // Vanguard string to itself

		EXPECT_EQ(vanguardFromLiteral, stdFromVanguard); // Vanguard string with std string
		EXPECT_EQ(vanguardFromLiteral, stdFromLiteral);
		EXPECT_EQ(stdFromVanguard, vanguardFromLiteral); // Flip lhs with rhs
		EXPECT_EQ(stdFromLiteral, vanguardFromLiteral);

		EXPECT_EQ(vanguardFromLiteral, juceFromVanguard); // Vanguard string with juce string
		EXPECT_EQ(vanguardFromLiteral, juceFromLiteral);
		EXPECT_EQ(juceFromVanguard, vanguardFromLiteral); // Flip lhs with rhs
		EXPECT_EQ(juceFromLiteral, vanguardFromLiteral);

		EXPECT_EQ(vanguardFromLiteral, charPointerFromVanguard); // Vanguard string with const char*
		EXPECT_EQ(vanguardFromLiteral, charPointerFromLiteral);
		EXPECT_EQ(charPointerFromVanguard, vanguardFromLiteral); // Flip lhs with rhs
		EXPECT_EQ(charPointerFromLiteral, vanguardFromLiteral);

		EXPECT_EQ(vanguardFromLiteral, "test"); // Vanguard string with string literal
		EXPECT_EQ("test", vanguardFromLiteral); // Flip lhs with rhs

		// Verify we can go from other types back to vanguard as well.
		EXPECT_EQ("test", vanguardFromStd);
		EXPECT_EQ("test", vanguardFromJuce);
		EXPECT_EQ("test", vanguardFromCharPointer);
	}

	TEST_F(StringTest, ToUpperAndLower)
	{
		string mixed = "TeSt";
		string upper = "TEST";
		string lower = "test";

		EXPECT_EQ("TEST", mixed.ToUpper());
		EXPECT_EQ("test", mixed.ToLower());

		EXPECT_EQ(mixed.ToUpper(), lower.ToUpper());
		EXPECT_EQ(mixed.ToLower(), upper.ToLower());
	}

	TEST_F(StringTest, StringSplit)
	{
		string conjoinedString = "Thing1;Thing2;Thing3;Thing4";

		List<string> splitString = conjoinedString.Split(';');

		EXPECT_EQ(4, splitString.size());

		EXPECT_EQ("Thing1",splitString[0]);
		EXPECT_EQ("Thing2", splitString[1]);
		EXPECT_EQ("Thing3", splitString[2]);
		EXPECT_EQ("Thing4", splitString[3]);
	}

	TEST_F(StringTest, RemoveAndReplace)
	{
		string helloString = "Hello";
		string helloString_ReplacedLwithD = helloString.Replace('l', 'd');
		EXPECT_EQ("Heddo", helloString_ReplacedLwithD);

		string smartString = "I am Smart";
		string notSmartString = smartString.Replace("Smart", "Stupid");
		EXPECT_EQ("I am Stupid", notSmartString);

		string goodAdvice = "Don't run with scissors";
		string badAdvice = goodAdvice.Remove("n't");
		EXPECT_EQ("Do run with scissors", badAdvice);

		string whatString = "Whhhhhhhat";
		string watString = whatString.Remove('h');
		EXPECT_EQ("Wat", watString);

		string stringWithLotsOfGrabage = "Howdy there partner#!@)$)!#@%)%$#&^&$%^))!()@!";
		string stringWithoutGarbage = stringWithLotsOfGrabage.RemoveCharacters("!@#$%^&*()");
		EXPECT_EQ("Howdy there partner", stringWithoutGarbage);
	}

	TEST_F(StringTest, Trimming)
	{
		string stringWithWhitespace = "  \n\n\n test string      \n    \n\n";
		string trimmedString = stringWithWhitespace.Trim();
		EXPECT_EQ("test string", trimmedString);

		string stringNeedingTrimming = "+_THE_STRING_+";

		string stringEndTrimmed = stringNeedingTrimming.TrimEnd("_+");
		EXPECT_EQ("+_THE_STRING", stringEndTrimmed);

		string stringStartTrimmed = stringNeedingTrimming.TrimStart("_+");
		EXPECT_EQ("THE_STRING_+", stringStartTrimmed);
	}

	TEST_F(StringTest, BooleanConversion)
	{
		string trueString = string::FromBoolean(true);
		string falseString = string::FromBoolean(false);		

		EXPECT_EQ(string::TRUE_STRING, trueString);
		EXPECT_EQ(string::FALSE_STRING, falseString);

		EXPECT_NE(string::TRUE_STRING, falseString);
		EXPECT_NE(string::FALSE_STRING, trueString);


		EXPECT_TRUE(string("true").ToBoolean());
		EXPECT_TRUE(string("TRUE").ToBoolean());
		EXPECT_TRUE(string("TrUe").ToBoolean());

		EXPECT_FALSE(string("false").ToBoolean());
		EXPECT_FALSE(string("FALSE").ToBoolean());
		EXPECT_FALSE(string("fAlSe").ToBoolean());
	}

	TEST_F(StringTest, IntegerConversion)
	{
		EXPECT_EQ("15", string::FromInt32(15));
		EXPECT_EQ("-64", string::FromInt32(-64));

		EXPECT_EQ(87, string("87").ToInt32());
		EXPECT_EQ(-42, string("-42").ToInt32());
	}

	TEST_F(StringTest, FloatConversion)
	{
		EXPECT_EQ("15", string::FromFloat(15));
		EXPECT_EQ("-64", string::FromFloat(-64));
		EXPECT_EQ("-7.64", string::FromFloat(-7.64f));
		EXPECT_EQ("6000", string::FromFloat(6000));


		EXPECT_EQ(87, string("87.0f").ToFloat());
		EXPECT_EQ(-42.f, string("-42").ToFloat());
		EXPECT_EQ(0.002156f, string("0.002156f").ToFloat());
		EXPECT_EQ(100.0f, string("100").ToFloat());

	}
}