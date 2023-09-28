CC=g++
FLAGS=-std=c++20 -Wall -pedantic -Werror -Wextra
TEST_LIB=-lgtest -lgtest_main -lpthread


all: allocator tests

allocator: allocator.cpp allocator.hpp
	$(CC) $(FLAGS) -c allocator.cpp

tests: allocator.o tests.cpp
	$(CC) $(FLAGS) allocator.o tests.cpp -o tests $(TEST_LIB)

test:
	./tests

clean:
	rm -f *.o tests