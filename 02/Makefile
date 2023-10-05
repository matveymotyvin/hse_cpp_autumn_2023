CC=g++
FLAGS=-std=c++20 -g -Wall -pedantic -Wextra
TEST_LIB=-lgtest -lgtest_main -lpthread

all: parser test_bin test

parser: parser.cpp parser.hpp
	$(CC) $(FLAGS) -c parser.cpp -o parser

test_bin: parser.o tests.o
	$(CC) $(FLAGS) tests.o  parser.o -o test_bin $(TEST_LIB)

tests.o: parser.o
	$(CC) $(FLAGS) -c tests.cpp

test:
	./test_bin

clean:
	rm -r *.o test_bin parser



