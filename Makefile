.PHONY: all, clean

all: hashmap.o hashmap_tests.o
	ar rcs -o libhashmap.a hashmap.o
	ar rcs -o libhashmap_tests.a hashmap_tests.o

hashmap.o: hashmap.c hashmap.h vector.c vector.h pair.h pair.c hash_funcs.h
	gcc -c -Wall -Wextra -Wvla -Werror -g -lm -std=c99 hashmap.c

hashmap_tests.o: hashmap.c hashmap.h vector.c vector.h test_suite.c test_suite.h pair.h pair.h hash_funcs.h test_pairs.h
	gcc -c -Wall -Wextra -Wvla -Werror -g -lm -std=c99 test_suite.c -o hashmap_tests

clean:
	rm *.o *.exe