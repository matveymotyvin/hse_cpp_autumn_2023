CC=g++
FLAGS=-std=c++20 -Wall -pedantic -Werror -Wextra
TEST_LIB=-lgtest -lgtest_main -lpthread

all: matrix tests

matrix: matrix.cpp matrix.hpp
	$(CC) $(FLAGS) -c matrix.cpp

tests: matrix.o tests.cpp
	$(CC) $(FLAGS) matrix.o tests.cpp -o tests $(TEST_LIB)

test:
	./tests

clean:
	rm -f *.o tests
