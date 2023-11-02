CC=g++
FLAGS=-std=c++20 -Wall -pedantic -Werror -Wextra
TEST_LIB=-lgtest -lgtest_main -lpthread

all: serial test_bin

serial: serial.cpp serial.hpp
	$(CC) $(FLAGS) -c serial.cpp

test_bin: serial.o tests.cpp
	$(CC) $(FLAGS) serial.o tests.cpp -o test_bin $(TEST_LIB)

test:
	./test_bin

clean:
	rm -f *.o test_bin
