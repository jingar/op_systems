#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <pthread.h>


main()
{
  int i = 0;
  int a[1000] = {[0 ... 999] = 1};
  int b[1000] = {[0 ... 999] = 1};
  
  for(i; i < 1000;i++)
    {
      printf("%d %d \n",i,array[i]);
    }
  
}
