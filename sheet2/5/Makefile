CFLAGS := -Wall -Wextra -pedantic -std=c99 -D_GNU_SOURCE -D_BSD_SOURCE

all: search

search: search.o
	$(CC) $(CFLAGS) $^ -o $@  -lrt -pthread
search.o: search.c

clean:
	$(RM) *.o search
