CFLAGS := -g -Wall -Wextra -std=c99 -D_GNU_SOURCE -D_BSD_SOURCE

all: parallel	

parallel: parallel.c
	$(CC) $^ -o $@ -lrt -pthread

clean:
	$(RM) *.o parallel