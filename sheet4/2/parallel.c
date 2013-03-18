#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <malloc.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <limits.h>
#include <pthread.h>

#define SIZE 1000
#define N_INTERATIONS 100000

struct timespec start, finish;
  
typedef struct _thread_args{
  pthread_t id;
  int from;
  int to;
}thread_args;

thread_args** thread_args_ptr;
//typedef thread_args **thread_args_ptr;

int a[SIZE] = {[0 ... 999] = 1};
int b[SIZE] = {[0 ... 999] = 1};
int first = 0;
int rest = 0;
int count_test_par =0;
int count_test_seq =0;

/* Time difference between a and b in microseconds, taken from bodo's coursework*/
static int64_t
xelapsed (struct timespec a, struct timespec b)
{
  return ((int64_t)a.tv_sec - b.tv_sec) * 1000000
    + ((int64_t)a.tv_nsec - b.tv_nsec) / 1000LL;
}
void add_arrays_seq()
{
  int i,j;
  for(j = 0; j < 100000;j++)
    {
      for(i = 0; i < SIZE;i++)
	{
	  a[i] = a[i] + b[i];    
	  count_test_seq++;
	}
    }
}
void* add_arrays(void* args)
{
  //int* i = args;
  //printf("add array arg %d \n",*i);
  //printf("add arrays_seq from %d to %d\n",from,to);
  //initialise a and by to size 1000 and make each element equal to 1
  int i = ((thread_args*)args)->from;
  int to = ((thread_args*)args)->to;
  int j;
  for(j = 0; j < 1000000;j++)
    {
      for(i; i < to;i++)
	{
	  a[i] = a[i] + b[i];    
	  count_test_par++;
	}
    }
}

void split_work(int number)
{
  int i;
  int counter = 0;
  div_t n = div(SIZE,number);
  thread_args_ptr[0]->from = 0;
  if(n.rem >0)
    {
      counter = thread_args_ptr[0]->to = (n.quot + n.rem)-1;
    }
  else
    {
      counter = thread_args_ptr[0]->to = n.quot-1;
    }
  //printf("from %d to %d \n",thread_args_ptr[0]->from ,thread_args_ptr[0]->to);
  for(i = 1; i < number; i++)
    {
      thread_args_ptr[i]->from = counter+1;
      thread_args_ptr[i]->to = counter + n.quot;
      printf(" from %d to %d \n", thread_args_ptr[i]->from,thread_args_ptr[i]->to);
      counter+=n.quot;
    }
}

void parallel_add(int number_of_threads)
{
  int thread_check;
  int i = 0;
  
  thread_args_ptr = malloc(number_of_threads * sizeof(thread_args*));
  for (i = 0 ; i<number_of_threads ; i++) 
    {
      thread_args_ptr[i] = malloc(sizeof(thread_args));
    }

  if (thread_args_ptr == NULL)
    {
      printf("ERROR: Out of memory\n");
      exit(1);
    }  

  split_work(number_of_threads);
  thread_check = pthread_create(&thread_args_ptr[0]->id,NULL,add_arrays,thread_args_ptr[0]);
  if(thread_check != 0)
    {
      printf("Thread could not be created");
    }


  for (i = 1;i < number_of_threads; i++)
    {     
      thread_check = pthread_create(&thread_args_ptr[i]->id,NULL,add_arrays,thread_args_ptr[i]);

      if(thread_check != 0)
	{
	  printf("Thread could not be created");
	}
      //printf("%lu test \n",thread_args_ptr[i]->id);
    }
  for(i=0; i>number_of_threads; i++)
    { 
      if(pthread_join(thread_args_ptr[i]->id, NULL) != 0) 
	{ 
	  perror("Thread not created\n");
	  exit(1);
	} 
    }
  free(thread_args_ptr);
}

void print_array()
{
  int i;
  for(i=0; i<SIZE;i++)
    {
      printf("%d %d",i,a[i]);
    }
}

int main(int argc, char** argv)
{
  if(!(argc < 2))
    {
      int number_of_threads = atoi(argv[1]);
      printf("%d \n",number_of_threads);
      clock_gettime(CLOCK_REALTIME, &start);
      
      parallel_add(number_of_threads);

      clock_gettime(CLOCK_REALTIME, &finish);
      fprintf (stderr, "Total time: %03li\n", xelapsed (finish, start));

      clock_gettime(CLOCK_REALTIME, &start);
      
      add_arrays_seq(number_of_threads);

      clock_gettime(CLOCK_REALTIME, &finish);
      fprintf (stderr, "Total time: %03li\n", xelapsed (finish, start));
    }
  else{
    //printf("must be at least one thread \n");
  }
  printf("COUNT TEST %d \n",count_test_seq);
  printf("COUNT TEST %d \n",count_test_par);

  //print_array();

}
