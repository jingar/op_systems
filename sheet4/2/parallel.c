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
#define N_ITERATIONS 100000
#define N_THREADS 16

struct timespec s_start, s_finish,p_start,p_finish;
  
typedef struct t_info{
  pthread_t id;
  int from;
  int to;
}thread_args;
//array to store thread_args pointers
thread_args** thread_args_ptr;


int a[SIZE];
int b[SIZE];


/* Time difference between a and b in microseconds, taken from bodo's coursework*/
static int64_t
xelapsed (struct timespec a, struct timespec b)
{
  return ((int64_t)a.tv_sec - b.tv_sec) * 1000000
    + ((int64_t)a.tv_nsec - b.tv_nsec) / 1000LL;
}

//sequentially adds arrays a and b, such that a[n] += b[n]. This adding is carried out N_ITERATION TIMES 
void* add_arrays_seq()
{
  int i,j;

  for(j = 0; j < N_ITERATIONS;j++)
    {
      for(i = 0; i < SIZE;i++)
	{
	  a[i] = a[i] + b[i];    
	}
  }
}

//parallel version of add_arrays_seq, takes in a struct thread_args
void *add_arrays(void* args)
{
  //each thread carriers out the addition for a chunk, chunk is descide from 
  int from = ((thread_args*)args)->from;
  int to = ((thread_args*)args)->to;
  int i,j;
  for(j = 0; j < N_ITERATIONS;j++)
    {
      for(i=from; i <= to;i++)
	{
	  a[i] = a[i] + b[i];    
	}
    }
}
//split the amount of work each thread has to do evenly as possible
void split_work(int number)
{
  int i;
  int counter = 0;
  //n is now the quotient and the remainder
  div_t n = div(SIZE,number);
  //initialize the first thread as it is always starting from position 0
  thread_args_ptr[0]->from = 0;
  //if there is a remainder then it was not possible to split the work evenly there for the first thread does the work equivelent to
  // the SIZE/thread + the remainder
  //counter holds the initial starting point for every thread other than the first thread.
  if(n.rem >0)
    {
      counter = thread_args_ptr[0]->to = (n.quot + n.rem);
    }
  //otherwise the first thread does work the same work as all the other threads
  else
    {
      //n.quot - 1 because if the the quot is 7 we actually want to do work in terms of index from 0 to 6
      counter = thread_args_ptr[0]->to = n.quot-1;
    }
  //split for all the other threads, use counter to keep track of how much to shift by
  for(i = 1; i < number; i++)
    {
      //+1 is used so that if the previous chunk is from 0-7, the next chuck will be 8 onwards
      thread_args_ptr[i]->from = counter+1;
      thread_args_ptr[i]->to = counter + n.quot;
      counter+=n.quot;
    }
}

void parallel_add(int number_of_threads)
{
  int thread_check;
  int i = 0;
  //allocate space for the thread_args_ptr which store pointers to thread_args
  thread_args_ptr = malloc(number_of_threads * sizeof(thread_args*));
  //check if the memory allocation was successfull
  if (thread_args_ptr == NULL)
    {
      printf("ERROR: Out of memory\n");
      exit(1);
    }
  for (i = 0 ; i<number_of_threads; i++)
    {
      //allocate space for each structs and assign each pointer in thread_args_ptr to the thread_args, if it returns NULL
      //print error and exit
      if((thread_args_ptr[i] = malloc(sizeof(thread_args))) == NULL)
	{
	  printf("ERROR: Out of memory\n");
	  exit(1);
	}
    }
  //split the work for each thread
  split_work(number_of_threads);
  //initialise the first thread, this is done because the first thread may differ in work from other threads
  thread_check = pthread_create(&thread_args_ptr[0]->id,NULL,add_arrays,thread_args_ptr[0]);
  //check if the thread creation worked
  if(thread_check != 0)
    {
      printf("Thread could not be created");
    }
  //initialise rest of the threads, this only works if the required number of threads is larger than 1
  for (i = 1;i < number_of_threads; i++)
    {     
      //create a thread , store the id in position i, and pass in the arguments that are postion i
      thread_check = pthread_create(&thread_args_ptr[i]->id,NULL,add_arrays,thread_args_ptr[i]);
      //check if the thread creation worked
      if(thread_check != 0)
	{
	  perror("Thread could not be created");
	}
    }
  
  //join all the threads together , if the join does work return error and exit
  for(i=0; i<number_of_threads; i++)
    {
      if(pthread_join(thread_args_ptr[i]->id, NULL) != 0) 
	{ 
	  perror("Thread not joined\n");
	  exit(1);
	} 
    }
  //free all the structs created
  for(i=0; i<number_of_threads; i++) 
    { 
      free(thread_args_ptr[i]); 
    }
  //free the array holding pointers to the structs
  free(thread_args_ptr);
}

//Initialise data in array a and b
void init_data( )
{
  int i;
  for( i=0; i<SIZE; i++) 
    {
      a[i] = i;
      b[i] = (SIZE-1) - i;
    }
}

int main(int argc, char** argv)
{
  if(!(argc < 2))
    {
      int number_of_threads = atoi(argv[1]);
      //initialise data for arrays a and b
      init_data();
  
      clock_gettime(CLOCK_REALTIME, &s_start);
      parallel_add(number_of_threads);
      
      clock_gettime(CLOCK_REALTIME, &s_finish);
      fprintf (stderr, "Total time par: %03li\n", xelapsed (s_finish, s_start));

      
      clock_gettime(CLOCK_REALTIME, &p_start);
      
      add_arrays_seq(number_of_threads);
      
      clock_gettime(CLOCK_REALTIME, &p_finish);
      fprintf (stderr, "Total time seq: %03li\n", xelapsed (p_finish, p_start));
    }
  else
    {
      printf("must be at least one thread \n");
    }
}
