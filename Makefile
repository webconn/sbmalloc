CFLAGS=-std=c99 -Wall -Werror

sbmalloc.c: sbmalloc.h config.h mutex.h mutex_t.h

test/test.c: test/tinytest.h sbmalloc.h

test/test.o: test/test.c
	$(CC) -c -o $@ $^ $(CFLAGS) -I.

test/test: test/test.o sbmalloc.o
	$(CC) -o $@ $^ $(LDFLAGS)

test: test/test
	test/test

clean:
	rm test/test test/*.o *.o -rf

.PHONY: test clean
