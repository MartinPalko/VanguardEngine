#include "gtest/gtest.h"
#include "Types/VanguardString.h"

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
		String vanguardFromLiteral = "test";
		String vanguardFromLiteral2 = "test";

		std::string stdFromVanguard = vanguardFromLiteral;
		std::string stdFromLiteral = "test";

		juce::String juceFromVanguard = vanguardFromLiteral;
		juce::String juceFromLiteral = "test";

		const char* charPointerFromVanguard = vanguardFromLiteral.GetCharPointer();
		const char* charPointerFromLiteral = "test";

		String vanguardFromStd = stdFromLiteral;
		String vanguardFromJuce = juceFromLiteral;
		String vanguardFromCharPointer = charPointerFromLiteral;

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

	TEST_F(StringTest, MiscQuery)
	{
		String testString = "0123456789";
		EXPECT_EQ(10, testString.GetLength());

		String emptyString = "   \n\n    \n";
		EXPECT_TRUE(emptyString.IsEmpty());
	}

	TEST_F(StringTest, IndexAccess)
	{
		String testString = "0123456789";

		EXPECT_EQ('4', testString[4]);
		EXPECT_EQ('9', testString[9]);
	}

	TEST_F(StringTest, Searching)
	{
		String testString = "abc123abc";

		EXPECT_TRUE(testString.Contains('a'));
		EXPECT_FALSE(testString.Contains('f'));

		EXPECT_TRUE(testString.Contains("c123"));
		EXPECT_TRUE(testString.Contains("abc"));
		EXPECT_FALSE(testString.Contains("cds"));
		EXPECT_FALSE(testString.Contains("abc324"));

		EXPECT_TRUE(testString.ContainsAny("cds"));
		EXPECT_FALSE(testString.ContainsAny("fgw"));

		EXPECT_EQ(0, testString.FirstIndexOf('a'));
		EXPECT_EQ(-1, testString.FirstIndexOf('w'));

		EXPECT_EQ(3, testString.FirstIndexOfAny("312"));
		EXPECT_EQ(-1, testString.FirstIndexOfAny("946ASD"));

		EXPECT_EQ(6, testString.LastIndexOf('a'));
		EXPECT_EQ(-1, testString.LastIndexOf('w'));

		EXPECT_EQ(5, testString.LastIndexOfAny("312"));
		EXPECT_EQ(-1, testString.LastIndexOfAny("946ASD"));

		EXPECT_EQ(1, testString.FirstIndexNotOf('a'));
		EXPECT_EQ(0, testString.FirstIndexNotOf('c'));

		EXPECT_EQ(3, testString.FirstIndexNotOfAny("cba"));
		EXPECT_EQ(0, testString.FirstIndexNotOfAny("123"));

		EXPECT_EQ(7, testString.LastIndexNotOf('c'));
		EXPECT_EQ(8, testString.LastIndexNotOf('a'));

		EXPECT_EQ(5, testString.LastIndexNotOfAny("asdfbc"));
		EXPECT_EQ(8, testString.LastIndexNotOfAny("wasd"));

		EXPECT_TRUE(testString.BeginsWith('a'));
		EXPECT_FALSE(testString.BeginsWith('s'));

		EXPECT_TRUE(testString.EndsWith('c'));
		EXPECT_FALSE(testString.EndsWith('s'));

		EXPECT_TRUE(testString.BeginsWithAny("asf"));
		EXPECT_FALSE(testString.BeginsWithAny("sdrf"));

		EXPECT_TRUE(testString.EndsWithAny("cas"));
		EXPECT_FALSE(testString.EndsWithAny("swg"));
	}

	TEST_F(StringTest, Appending)
	{
		String stringOne = "one";
		String stringTwo = "two";

		EXPECT_EQ("onetwo", stringOne.Append(stringTwo));
		EXPECT_EQ("onetwo", stringOne + stringTwo);

		EXPECT_EQ("one test", stringOne + " test");
	}

	TEST_F(StringTest, ToUpperAndLower)
	{
		String mixed = "TeSt";
		String upper = "TEST";
		String lower = "test";

		EXPECT_EQ("TEST", mixed.ToUpper());
		EXPECT_EQ("test", mixed.ToLower());

		EXPECT_EQ(mixed.ToUpper(), lower.ToUpper());
		EXPECT_EQ(mixed.ToLower(), upper.ToLower());
	}

	TEST_F(StringTest, StringSplitAndJoin)
	{
		String conjoinedString = "Thing1;Thing2;Thing3;Thing4";

		DynamicArray<String> splitString = conjoinedString.Split(';');

		EXPECT_EQ(4, splitString.Count());

		EXPECT_EQ("Thing1",splitString[0]);
		EXPECT_EQ("Thing2", splitString[1]);
		EXPECT_EQ("Thing3", splitString[2]);
		EXPECT_EQ("Thing4", splitString[3]);

		String rejoinedString = String::Join(splitString, ';');

		EXPECT_EQ(conjoinedString, rejoinedString);
	}

	TEST_F(StringTest, RemoveReplaceAndInsert)
	{
		String helloString = "Hello";
		String helloString_ReplacedLwithD = helloString.Replace('l', 'd');
		EXPECT_EQ("Heddo", helloString_ReplacedLwithD);

		String smartString = "I am smart";
		String notSmartString = smartString.Replace("smart", "stupid");
//		EXPECT_EQ("I am stupid", notSmartString);
//
//		String goodAdvice = "Don't run with scissors";
//		String badAdvice = goodAdvice.Remove("n't");
//		EXPECT_EQ("Do run with scissors", badAdvice);
//
//		String whatString = "Whhhhhhhat";
//		String watString = whatString.Remove('h');
//		EXPECT_EQ("Wat", watString);
//
//		String stringWithLotsOfGrabage = "Howdy there partner#!@)$)!#@%)%$#&^&$%^))!()@!";
//		String stringWithoutGarbage = stringWithLotsOfGrabage.RemoveCharacters("!@#$%^&*()");
//		EXPECT_EQ("Howdy there partner", stringWithoutGarbage);
//
//		String nonsenseAdvice = goodAdvice.Remove(5, 9);
//		EXPECT_EQ("Don't scissors", nonsenseAdvice);
//
//		String sureBud = smartString.RemoveBetween(1, 4);
//		String iBeleiveYou = smartString.RemoveBetween(4, 1);
//		EXPECT_EQ("I smart", sureBud);
//		EXPECT_EQ("I smart", iBeleiveYou);
//
//		String motivationalString = "You can do it!";
//		String demotivationalString = motivationalString.Insert(7, " not");
//
//		EXPECT_EQ("You can not do it!", demotivationalString);
//
//		EXPECT_EQ("He", helloString.RemoveAfter(helloString.FirstIndexOf('l') - 1));
//		EXPECT_EQ("lo", helloString.RemoveBefore(2));
	}

	TEST_F(StringTest, Trimming)
	{
		String stringWithWhitespace = "  \n\n\n test string      \n    \n\n";
		String trimmedString = stringWithWhitespace.Trim();
		EXPECT_EQ("test string", trimmedString);

		String stringNeedingTrimming = "+_THE_STRING_+";

		String stringEndTrimmed = stringNeedingTrimming.TrimEnd("_+");
		EXPECT_EQ("+_THE_STRING", stringEndTrimmed);

		String stringStartTrimmed = stringNeedingTrimming.TrimStart("_+");
		EXPECT_EQ("THE_STRING_+", stringStartTrimmed);
	}

	TEST_F(StringTest, BooleanConversion)
	{
		String trueString = String::FromBoolean(true);
		String falseString = String::FromBoolean(false);

		EXPECT_EQ(TRUE_STRING, trueString);
		EXPECT_EQ(FALSE_STRING, falseString);


		EXPECT_TRUE(String("true").ToBoolean());
		EXPECT_TRUE(String("TRUE").ToBoolean());
		EXPECT_TRUE(String("TrUe").ToBoolean());

		EXPECT_FALSE(String("false").ToBoolean());
		EXPECT_FALSE(String("FALSE").ToBoolean());
		EXPECT_FALSE(String("fAlSe").ToBoolean());
	}

	TEST_F(StringTest, IntegerConversion)
	{
		EXPECT_EQ("15", String::FromInt32(15));
		EXPECT_EQ("-64", String::FromInt32(-64));

		EXPECT_EQ(87, String("87").ToInt32());
		EXPECT_EQ(-42, String("-42").ToInt32());
	}

	TEST_F(StringTest, FloatConversion)
	{
		EXPECT_EQ("15", String::FromFloat(15));
		EXPECT_EQ("-64", String::FromFloat(-64));
		EXPECT_EQ("-7.64", String::FromFloat(-7.64f));
		EXPECT_EQ("6000", String::FromFloat(6000));


		EXPECT_EQ(87, String("87.0f").ToFloat());
		EXPECT_EQ(-42.f, String("-42").ToFloat());
		EXPECT_EQ(0.002156f, String("0.002156f").ToFloat());
		EXPECT_EQ(100.0f, String("100").ToFloat());
	}
}
