CC=g++
FLAGS=-std=c++20 -Wall -pedantic -Werror -Wextra
TEST_LIB=-lgtest -lgtest_main -lpthread

all: formater test_bin

formater: formater.cpp formater.hpp
	$(CC) $(FLAGS) -c formater.cpp

test_bin: formater.o tests.cpp
	$(CC) $(FLAGS) formater.o tests.cpp -o test_bin $(TEST_LIB)

test:
	./test_bin

clean:
	rm -f *.o test_bin
