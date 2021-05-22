.PHONY: all, clean

all: hashmap.o vector.o pair.o hashmap_tests.o
	ar rcs -o libhashmap.a hashmap.o vector.o pair.o
	ar rcs -o libhashmap_tests.a hashmap_tests.o hashmap.o vector.o pair.o

hashmap.o: hashmap.c hashmap.h hash_funcs.h
	gcc -c -Wall -Wextra -Wvla -Werror -g -lm -std=c99 hashmap.c

vector.o: vector.h vector.c
	gcc -c -Wall -Wextra -Wvla -Werror -g -lm -std=c99 vector.c

pair.o: pair.h pair.c
	gcc -c -Wall -Wextra -Wvla -Werror -g -lm -std=c99 pair.c

hashmap_tests.o: test_suite.c test_suite.h hash_funcs.h test_pairs.h
	gcc -c -Wall -Wextra -Wvla -Werror -g -lm -std=c99 test_suite.c -o hashmap_tests.o



clean:
	rm *.o *.exe