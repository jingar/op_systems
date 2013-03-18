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

struct timespec start, finish;
  
typedef struct t_info{
  pthread_t id;
  int from;
  int to;
}thread_args;
//typedef struct t_info thread_args;
thread_args** thread_args_ptr;


int a[SIZE];
int b[SIZE];
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
void* add_arrays_seq()
{
  int i,j;
  for(j = 0; j < N_ITERATIONS;j++)
    {
      for(i = 0; i < SIZE;i++)
	{
	  a[i] = a[i] + b[i];    
	  count_test_seq++;
	}
  
  }
}
void *add_arrays(void* args)
{
  //int* i = args;
  //printf("add array arg %d \n",*i);
  //printf("add arrays_seq from %d to %d\n",from,to);
  //initialise a and by to size 1000 and make each element equal to 1
  //  printf("IN ADD ARRAYS id %lu, from %d, to %d \n",((thread_args*)args)->id,((thread_args*)args)->from,((thread_args*)args)->to);
  int from = ((thread_args*)args)->from;
  int to = ((thread_args*)args)->to;
  int i,j;
  for(j = 0; j < N_ITERATIONS;j++)
    {
      for(i=from; i <= to;i++)
	{
	  a[i] = a[i] + b[i];    
	  //printf("a[%d] = %d \n",i,a[i]);
	}
    }
}

void split_work(int number)
{
  int i;
  int counter = 0;
  div_t n = div(SIZE,number);
  thread_args_ptr[0]->from = 0;
  //printf(" SIZE %d, number %d,quot %d , rem %d \n",SIZE,number,n.quot,n.rem);
  if(n.rem >0)
    {
      counter = thread_args_ptr[0]->to = (n.quot + n.rem);
      //printf("counter if rem > 0 %d \n",counter);
    }
  else
    {
      counter = thread_args_ptr[0]->to = n.quot-1;
      //printf("counter else %d \n",counter);
    }
  printf("first from %d to %d \n", thread_args_ptr[0]->from,thread_args_ptr[0]->to);
  for(i = 1; i < number; i++)
    {
      thread_args_ptr[i]->from = counter+1;
      thread_args_ptr[i]->to = counter + n.quot;
      //thread_args_ptr[i]->from = thread_args_ptr[i-1]->to;
      //thread_args_ptr[i]->to = thread_args_ptr[i]->from + n.quot;
      printf(" from %d to %d \n", thread_args_ptr[i]->from,thread_args_ptr[i]->to);
      counter+=n.quot;
    }
}

void parallel_add(int number_of_threads)
{
  //  printf("START PARRALEL \n");
  int thread_check;
  int i = 0;

  thread_args_ptr = malloc(number_of_threads * sizeof(thread_args*));
  for (i = 0 ; i<number_of_threads; i++)
    {
      thread_args_ptr[i] = malloc(sizeof(thread_args));
      //  printf("START ALLOCATION MEMORY \n");
    }
  if (thread_args_ptr == NULL)
    {
      printf("ERROR: Out of memory\n");
      exit(1);
    }  
  
  split_work(number_of_threads);
  //printf("AFTER SPLIT \n");
  thread_check = pthread_create(&thread_args_ptr[0]->id,NULL,add_arrays,thread_args_ptr[0]);
  //printf("args id %lu first from %d to %d \n", thread_args_ptr[0]->id,thread_args_ptr[0]->from,thread_args_ptr[0]->to);
  //printf("thread_check %d \n",thread_check);
  if(thread_check != 0)
    {
      printf("Thread could not be created");
    }
  for (i = 1;i < number_of_threads; i++)
    {     
      thread_check = pthread_create(&thread_args_ptr[i]->id,NULL,add_arrays,thread_args_ptr[i]);

      if(thread_check != 0)
	{
	  perror("Thread could not be created");
	}
      //printf("%lu test \n",thread_args_ptr[i]->id);
      }
 
  
  for(i=0; i<number_of_threads; i++)
    { 
      if(pthread_join(thread_args_ptr[i]->id, NULL) != 0) 
	{ 
	  perror("Thread not created\n");
	  exit(1);
	} 
    }
  for(i=0; i<number_of_threads; i++) 
    { 
      free(thread_args_ptr[i]); 
    }
  free(thread_args_ptr);
}

void print_array()
{
  int i;
  for(i=0; i<SIZE;i++)
    {
      printf("a %d b %d \n",a[i],b[i]);
    }
}

void best_time(int number_of_threads)
{
  int number,current_time,best_num_threads;
  int best_time = 1000000;
  for(number = 1; number <number_of_threads+1; number++)
    {
      clock_gettime(CLOCK_REALTIME, &start);
      parallel_add(number_of_threads);
      clock_gettime(CLOCK_REALTIME, &finish);
      current_time = xelapsed (finish, start);
      //printf("blaah %d\n",current_time);
      if(best_time > current_time)
	{
	  // printf("inside if c %d b %d\n",current_time,best_time);
	  best_time = current_time;
	  best_num_threads = number;
	}
    }
 p printf("Best time %d , Best number of threads %d \n",best_time,best_num_threads);
}
void best_thread_time(int number_of_threads)
{
  int number,current_time;
  int best_time = 1000000;
  for(number = 0; number < 20; number++)
    {
      clock_gettime(CLOCK_REALTIME, &start);
      parallel_add(number_of_threads);
      clock_gettime(CLOCK_REALTIME, &finish);
      current_time = xelapsed (finish, start);
      //printf("blaah %d\n",current_time);
      if(best_time > current_time)
	{
	  // printf("inside if c %d b %d\n",current_time,best_time);
	  best_time = current_time;
	}
    }
  printf("Best time %d , Best number of threads %d \n",best_time,number_of_threads);
}
void init_data( )
{
  int i;
  for( i=0; i<SIZE; i++) {
    a[i] = i;
    b[i] = (SIZE-1) - i;
  }
  //pthread_barrier_init(&barrier, NULL, N_THREADS);
}

int main(int argc, char** argv)
{
  if(!(argc < 2))
    {
      int number_of_threads = atoi(argv[1]);
      init_data();

      
      clock_gettime(CLOCK_REALTIME, &start);
      parallel_add(number_of_threads);
      
      clock_gettime(CLOCK_REALTIME, &finish);
      fprintf (stderr, "Total time: %03li\n", xelapsed (finish, start));

     
      
      clock_gettime(CLOCK_REALTIME, &start);
      
      add_arrays_seq(number_of_threads);
      
      clock_gettime(CLOCK_REALTIME, &finish);
      fprintf (stderr, "Total time: %03li\n", xelapsed (finish, start));
      // best_time(number_of_threads);
      //best_thread_time(number_of_threads);
    }
  else{
    printf("must be at least one thread \n");
  }
  //printf("COUNT TEST SEQ %d \n",count_test_seq);
  //printf("COUNT TEST PAR %d \n",count_test_par);

  //print_array();

}
