#include <gtest/gtest.h>
#include "formater.hpp"



class TestFoo : public ::testing::Test
{
    protected: 
        void SetUp() {}
        void TearDown() {}
};

// обычный тест
TEST(TestFormat, correct_check_1)
{
    std::stringstream stream;
    std::string line = "Hello, {0}. My name is {1} and sername is {2}";
    Formater format(stream, line);

    format("friend", "Matvey", "Motyavin");
    std::string res = format.getString();
    std::string corr = "Hello, friend. My name is Matvey and sername is Motyavin";
    ASSERT_EQ(corr, res);
}

// тест на форматер вместе с цифрой
TEST(TestFormat, correct_check_2)
{
    std::stringstream stream;
    std::string line = "{1}+{1} = {0}";
    Formater format(stream, line);

    format(2, "one");
    std::string res = format.getString();
    std::string corr = "one+one = 2";
    ASSERT_EQ(corr, res);
}

// тест только скобки
TEST(TestFormat, only_bracket)
{
    std::stringstream stream;
    std::string line = "{0} {1}";
    Formater format(stream, line);

    format("Matvey", "Motyavin");
    std::string res = format.getString();
    std::string corr = "Matvey Motyavin";
    ASSERT_EQ(corr, res);
}

// искулючение на неполную скобку
TEST(TestFormat, one_bracket_left) 
{
    std::stringstream stream;
    std::string line = "Hello, {0}. My name is {1} and sername is {2}  }";
    Formater format(stream, line);
    EXPECT_THROW({
        try
        {
            format("friend", "Matvey", "Motyavin");
            std::string res = format.getString();
        }
        catch( const BracketsMismatchException& e )
        {
            EXPECT_STREQ( "miss {}", e.what() );
            throw;
        }
    }, BracketsMismatchException );
}

// исключение на пустые скобки
TEST(TestFormat, space_bracket) 
{
   
    std::stringstream stream;
    std::string line = "Hello, {0}. My name is {} and sername is {}";
    Formater format(stream, line);
    EXPECT_THROW({
        try
        {
            format("friend", "Matvey", "Motyavin");
            std::string res = format.getString();
        }
        catch( const EmptyParamException& e )
        {
            EXPECT_STREQ( "miss format's index", e.what() );
            throw;
        }
    }, EmptyParamException );
}

// исключение на неправильное число параметров
TEST(TestFormat, param_number)
{
   std::stringstream stream;
    std::string line = "Hello, {0}. My name is {1} and sername is {2}, {3}, {4}"; 
    Formater format(stream, line);
    EXPECT_THROW({
        try
        {
            format("friend", "Matvey", "Motyavin");
            std::string res = format.getString();
        }
        catch( const IncorrectParamsNumberException& e )
        {
            EXPECT_STREQ( "order number params error", e.what() );
            throw;
        }
    }, IncorrectParamsNumberException );

}

// исключение на неправильнрый формат индексов
TEST(TestFormat, param_word) 
{
    std::stringstream stream;
    std::string line = "Hello, {0}. My name is {a} and sername is {b}";
    Formater format(stream, line);
    EXPECT_THROW({
        try
        {
            format("friend", "Matvey", "Motyavin");
            std::string res = format.getString();
        }
        catch( const NotANumberException& e )
        {
            EXPECT_STREQ( "format's index error value", e.what() );
            throw;
        }
    }, NotANumberException );
}


int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}