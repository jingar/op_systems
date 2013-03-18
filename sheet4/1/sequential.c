#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <pthread.h>

#define SIZE 1000

struct timespec start, finish;


/* Time difference between a and b in microseconds, taken from bodo's coursework*/
static int64_t
xelapsed (struct timespec a, struct timespec b)
{
  return ((int64_t)a.tv_sec - b.tv_sec) * 1000000
    + ((int64_t)a.tv_nsec - b.tv_nsec) / 1000LL;
}

main()
{
  int i,j;
  //initialise a and by to size 1000 and make each element equal to 1
  int a[SIZE] = {[0 ... 999] = 1};
  int b[SIZE] = {[0 ... 999] = 1};
  clock_gettime(CLOCK_REALTIME, &start);
    for(j = 0; j < 1000000;j++)
    {
      for(i = 0; i < SIZE;i++)
	{
	  a[i] = a[i] + b[i];    
	}
    }
  for(i=0; i<SIZE;i++)
    {
      printf("%d %d",i,a[i]);
    }

  clock_gettime(CLOCK_REALTIME, &finish);
  fprintf (stderr, "Total time: %03li\n", xelapsed (finish, start));
}
