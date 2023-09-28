#include <iostream>
#include <gtest/gtest.h>

#include "allocator.hpp"

class TestFoo : public ::testing::Test
{
protected:
    void SetUp() {}
    void TearDown() {}
};


TEST(TestAlloc, makeAllocator)
{
    Allocator al;
    al.makeAllocator( 20 ); // выделяем 15 байт

    {
        char *res = al.alloc( 10 );  // забираем 10 байт
        ASSERT_NE( res, nullptr ); // должно получиться
    }

    {
        char *res = al.alloc( 10 );  // забираем ещё 10 байт
        ASSERT_NE( res, nullptr ); // должно получиться
    }


    {
        char *res = al.alloc( 1 ); // пытаемся забрать 1 байт из полностью заполненного
        ASSERT_EQ( res, nullptr ); // должно не получиться
    }

}
TEST(TestAlloc, values)
{
    Allocator al;
    al.makeAllocator( 10 );// создаем аллокатор

    {
	ASSERT_EQ( al.offset, 0 ); // проверяем исходные поля
    }	

    {	
	char *res = al.alloc( 5 ); //аллоцируем  память 
	ASSERT_EQ( al.offset, 5 ); // проверяем смешение
	ASSERT_TRUE( res != nullptr ); // проверяем указатель
    }

    {
	char *res = al.alloc( 10 ); // пробуем аллоцировать больше положенного
	ASSERT_EQ( res, nullptr ); // должно не получиться
    }

}

TEST(TestAlloc, alloc)
{
    Allocator al;
    al.makeAllocator( 5 ); //выделяем 5 байт

    {
        char *res = al.alloc( 0 ); // пытаемся забрать 0 байт
        ASSERT_EQ( res, nullptr ); //память не выделялось
    }

    {
        char *res = al.alloc( 5 ); // пытаемся забрать 5 байт
        ASSERT_NE( res, nullptr ); // должно получиться
    }

    {
        char *res = al.alloc( 1 ); // пытаемся забрать 1 байт
        ASSERT_EQ( res, nullptr ); // должно не получиться
    }

}

TEST(TestAlloc, reset)
{
    Allocator al;
    al.makeAllocator( 20 ); // выделяем 20 байт

    {
        char *res = al.alloc( 20 ); // выделяем 20 байт
        ASSERT_TRUE( res != nullptr ); 
    }

    al.reset();


    {
        char *res = al.alloc( 1 ); // пытаемся забрать 1 байт
        ASSERT_TRUE( res != nullptr ); // должно получиться
    }
}


TEST(TestAlloc, allocatorFree)
{
    Allocator al;

    al.makeAllocator( 10 ); // вызываем аллокатор
    char *res = al.alloc( 1 );

    *res = '1'; // присваиваем какое-то значение указателю
    al.makeAllocator( 10 ); // повторно вызываем аллокатор, память должна освободиться и выделиться новая
    
    ASSERT_TRUE( *res != '1' ); // указатель должен измениться
    ASSERT_TRUE( res != nullptr );
    ASSERT_TRUE( al.offset == 0 );

}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
