#include <gtest/gtest.h>
#include "parser.hpp"
#include <string>
#include <math.h>

class TestFoo : public ::testing::Test
{
protected:
    void SetUp() {}
    void TearDown() {}
};

TEST(TestUtils, test_1)
{
	// test token callback, letter token callback and digit token callback
	TokenParser parser;
	std::string test = "first test 1 5 6";

	auto digit_callback = [](uint64_t x){return x*3;};
	parser.SetDigitTokenCallback(digit_callback);
	auto output_2 = parser.Parse(test);
	ASSERT_EQ(output_2, " first test 3 15 18 ");

	auto letter_callback = [](std::string s){return s.insert(0, "str_");};
    parser.SetLetterTokenCallback(letter_callback);
    auto output_3 = parser.Parse(test);
    ASSERT_EQ(output_3, " str_first str_test 3 15 18 ");
}

TEST(TestUtils, test_2)
{
    // test start callback and end callback
    TokenParser parser;
    auto start_callback = [](std::string s){return s.insert(7, "start_callback ");};
    parser.SetStartCallback(start_callback);
    std::string test = "second test 55";
    std::string output_1 = parser.Parse(test);
    ASSERT_EQ(output_1, " second start_callback test 55 ");

    auto end_callback = [](std::string s){return s.erase(0, 1);}; // del 1 spcae in the output string
    parser.SetEndCallback(end_callback);
    auto output_3 = parser.Parse(test);
    ASSERT_EQ(output_3, "second start_callback test 55 ");

}

TEST(TestUtils, test_3)
{
    // test special user's callbacks and too long digit parcing
    TokenParser parser;
    auto letter_callback = [](std::string s){return s.insert(0, "str_");};
    auto digit_callback = [](uint64_t x){return x*3;};

    parser.SetLetterTokenCallback(letter_callback);


    std::string spaces = "    ";
    std::string empty = "";

    auto spaces_output = parser.Parse(spaces);
    ASSERT_EQ(spaces_output, "    ");

    auto empty_output = parser.Parse(empty);
    ASSERT_EQ(empty_output, "");

    parser.SetDigitTokenCallback(digit_callback);
    std::string one_char_tokens_letter = "hi";
    auto one_char_tokens_letter_output = parser.Parse(one_char_tokens_letter);
    ASSERT_EQ(one_char_tokens_letter_output, " str_hi ");

    std::string one_char_tokens_digit = "1";
    auto one_char_tokens_digit_output = parser.Parse(one_char_tokens_digit);
    ASSERT_EQ(one_char_tokens_digit_output, " 3 ");

    std::string multimodal_tokens = "1 str1 2 str2 3str";
    auto multimodal_tokens_output = parser.Parse(multimodal_tokens);
    ASSERT_EQ(multimodal_tokens_output, " 3 str_str1 6 str_str2 str_3str ");

    auto digit_callback_ = [](uint64_t x){return x - 1;};
    parser.SetDigitTokenCallback(digit_callback_);

    std::string not_long = "18446744073709551615";
    auto not_long_output = parser.Parse(not_long);
    ASSERT_EQ(not_long_output, " 18446744073709551614 ");

    std::string too_long = "18446744073709551616";
    auto too_long_output = parser.Parse(too_long);
    ASSERT_EQ(too_long_output, " str_18446744073709551616 ");

    std::string very_long = "999999999999999999999999";
    auto very_long_output = parser.Parse(very_long);
    ASSERT_EQ(very_long_output, " str_999999999999999999999999 ");
}

TEST(TestUtils, all_nullptr)
{
    TokenParser parser;
    auto output = parser.Parse("null test");
    ASSERT_EQ(output, " null test ");
}


int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
