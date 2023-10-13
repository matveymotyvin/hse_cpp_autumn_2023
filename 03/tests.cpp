#include <iostream>
#include <gtest/gtest.h>

#include "matrix.hpp"

class TestFoo : public ::testing::Test
{
protected:
	void SetUp() {}
	void TearDown() {}
};

// Две одинаковые матрицы
TEST(TestUtils, test_1)
{
	Matrix m1(4, 1);
	Matrix m2(4, 6);

	ASSERT_EQ(m1 == m2, false);
}

// Третий элеммент ([0][2]) = 3
TEST(TestUtils, test_2)
{
	Matrix m(2, 3);
	for (int i = 0; i < 6; i++) m[i / 3][i % 3] = (i + 1);
	ASSERT_EQ(m[0][2], 3);
}

// Матрица с 2 строками
TEST(TestUtils, test_3)
{
	Matrix m(2, 3);
	ASSERT_EQ(m.getRows(), 2);
}

// Матрица с 3 столбцами
TEST(TestUtils, test_4)
{
	Matrix m(2, 3);
	ASSERT_EQ(m.getColumns(), 3);
}

// Домножение матрицы единиц на число
TEST(TestUtils, test_5)
{
	Matrix m(2, 3, 1);
	m *= 3.14;
	ASSERT_EQ(m[0][0], 3);
}

// Домножение, и вывод в поток
TEST(TestUtils, test_6)
{
	Matrix m(2, 3, 1);
	std::stringstream ss;
	m *= 3.14;
	ss << m;
	ASSERT_EQ(ss.str(), "[[3, 3, 3],\n[3, 3, 3]]");
}

// Сравнение матриц одной формы
TEST(TestUtils, test_7)
{
	Matrix m1(5, 8), m2(5, 8);
	ASSERT_EQ(m1 == m2, true);
}

// Сравнение матриц разной формы
TEST(TestUtils, test_8)
{
	Matrix m1(5, 8), m2(2, 3);
	ASSERT_EQ(m1 != m2, true);
}

// Сложение матриц из 1 и из 4, должны получиться 5
TEST(TestUtils, test_9)
{
	Matrix m1(5, 8, 1), m2(5, 8, 4), m3(5, 8, 5);
	ASSERT_EQ((m1 + m2) == m3, true);
}

// Сложение матриц из 1 и из 4, не должны получиться 6
TEST(TestUtils, test_10)
{
	Matrix m1(5, 8, 1), m2(5, 8, 4), m4(5, 8, 6);
	ASSERT_EQ((m1 + m2) != m4, true);
}

// Вычитание матрицы четверок из матрицы единиц
TEST(TestUtils, test_11)
{
	Matrix m1(5, 8, 1), m2(5, 8, 4), m3(5, 8, -3);
	ASSERT_EQ((m1 - m2) == m3, true);
}

// Домножение и сравнение матриц
TEST(TestUtils, test_12)
{
	Matrix m1(5, 8, 1), m2(5, 8, 4);
	m1 *= 4.86;
	ASSERT_EQ(m1 == m2, true);
}

// Вычитание и сравнение матриц
TEST(TestUtils, test_13)
{
	Matrix m1(5, 8, 1), m2(5, 8, 4), m3(5, 8, -3);
	ASSERT_EQ((m1 - m2) == m3, true);
}

int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
