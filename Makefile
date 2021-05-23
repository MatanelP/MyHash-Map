CC = gcc
CCFLAGS = -c -Wall -Wextra -Wvla -Werror -g -lm -std=c99
.PHONY: all, clean

all: hashmap.o vector.o pair.o hashmap_tests.o
	ar rcs -o libhashmap.a hashmap.o vector.o pair.o
	ar rcs -o libhashmap_tests.a hashmap_tests.o hashmap.o vector.o pair.o

hashmap.o: hashmap.c hashmap.h hash_funcs.h
	$(CC) $(CCFLAGS) hashmap.c

vector.o: vector.h vector.c
	$(CC) $(CCFLAGS)  vector.c

pair.o: pair.h pair.c
	$(CC) $(CCFLAGS)  pair.c

hashmap_tests.o: test_suite.c test_suite.h hash_funcs.h test_pairs.h
	$(CC) $(CCFLAGS)  test_suite.c -o hashmap_tests.o

clean:
	rm *.o *.exe