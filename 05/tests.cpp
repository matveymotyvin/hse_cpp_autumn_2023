#include <iostream>
#include <sstream>
#include <gtest/gtest.h>
#include "serial.hpp"


class TestFoo : public ::testing::Test
{
protected:
	void SetUp() {}
	void TearDown() {}
};

// тест на разные типы
TEST(TestUtils, test_1)
{
	Data x{ 5, true, 7 };

	std::stringstream stream;

	Serializer serializer(stream);
	serializer.save(x);

	Data y{ 0, false, 0 };
	Deserializer deserializer(stream);

	const Error err = deserializer.load(y);

	ASSERT_EQ(err, Error::NoError);
	ASSERT_EQ(x.a, y.a);
	ASSERT_EQ(x.b, y.b);
	ASSERT_EQ(x.c, y.c);
}

TEST(TestUtils, test_2)
{
	Data x{ 18231853722, false, 999999999999 };

	std::stringstream stream;

	Serializer serializer(stream);
	serializer.save(x);

	Data y{ 0, false, 0 };
	Deserializer deserializer(stream);

	const Error err = deserializer.load(y);

	ASSERT_EQ(err, Error::NoError);
	ASSERT_EQ(x.a, y.a);
	ASSERT_EQ(x.b, y.b);
	ASSERT_EQ(x.c, y.c);
}

//тест сериалайзера
TEST(TestUtils, test_3)
{
	Data x{ 5, true, 7 };

	std::stringstream stream;

	Serializer serializer(stream);
	serializer.save(x);

	ASSERT_EQ(stream.str(), "5 true 7 ");
}

// тест десириалайзера
TEST(TestUtils, test_4)
{
	std::stringstream stream("5 true 7 ");
	Data y{ 0, false, 0 };
	Deserializer deserializer(stream);

	const Error err = deserializer.load(y);

	ASSERT_EQ(err, Error::NoError);
	ASSERT_EQ(5, y.a);
	ASSERT_EQ(true, y.b);
	ASSERT_EQ(7, y.c);
}


int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
