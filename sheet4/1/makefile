CFLAGS := -g -Wall -Wextra -std=c99 -D_GNU_SOURCE -D_BSD_SOURCE

all: sequential

sequential: sequential.c
	$(CC) $^ -o $@ -lrt

clean:
	$(RM) *.o sequential