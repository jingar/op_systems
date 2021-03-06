#include <unistd.h>
#include <stdio.h>

#define MAX_SIZE 1073741824 //1gb in bytes
//#define MAX_SIZE 32



void* start_addr = NULL;
void* end_addr = NULL;
int count = 0;
int free_count =0;
typedef struct c_info
{
  size_t size;
  struct c_info *next;
  struct c_info *prev;
  void* ptr;
  int free;
}chunk_info;

const unsigned int CHUNK_SIZE = sizeof(chunk_info);

void* init()
{
	if((start_addr = sbrk(MAX_SIZE)) == ((void*)-1)) //failed to get memory
	{
		return NULL;
	}
	

	//end addr points to 1 byte past data segment, this is so end_addr - start_addr = useable memory
	end_addr = ((void*)(((long unsigned int)start_addr) + MAX_SIZE));
	printf("----END----%p",end_addr);
	chunk_info *first_chunk = (chunk_info*)start_addr;
		first_chunk->size = ((long unsigned int)end_addr) - (((long unsigned int)start_addr) + CHUNK_SIZE);
		first_chunk->next = NULL;
		first_chunk->prev = NULL;
		first_chunk->free = 1;

	/*
	printf("---------------------------\n");	 
	printf("Start address = %lu\n", ((long unsigned int)start_addr));
	printf("End address = %lu\n", ((long unsigned int)end_addr));
	printf("First chunk = %lu\n", ((long unsigned int)first_chunk));
	printf("Total memory = %i\n", MAX_SIZE);
	printf("Chunk size = %lu\n", CHUNK_SIZE);
	printf("Expected First chunk size = %lu\n", (MAX_SIZE - CHUNK_SIZE));
	printf("Actual First chunk size = %lu\n", (long unsigned int)first_chunk->size);
	printf("First chunk next = %lu\n", ((long unsigned int)first_chunk->next));
	printf("First chunk prev = %lu\n", ((long unsigned int)first_chunk->prev));
	printf("First Chunk free = %d\n", first_chunk->free);
	printf("---------------------------\n");
	*/

	return (int*)1;
}

void printHeap()
{
	chunk_info *temp = start_addr;
	printf("START----------START\n");
    while(temp != NULL)
    {
		printf("Chunk at: %lu\n", ((long unsigned int)temp));
		printf("Total Chunk size: %lu\n", ((long unsigned int)temp->size) + CHUNK_SIZE);
		printf("Expected next chunk at: %lu\n", ((long unsigned int)temp->size) + CHUNK_SIZE + ((long unsigned int)temp));
		printf("--------------------\n");
		temp = temp->next;
    }
}

chunk_info *findFirstFreeBlock(size_t size)
{
    chunk_info *temp = start_addr;
    int link_count = 0;
    while(!((temp->free == 1) && (temp->size >= size)))
 {
		if(temp->next !=NULL)
		{
			temp = temp->next;
			++link_count;
		}
		else
		{
			return NULL;
		}
    }
    //printf(" --TEMP-- %p \n",temp);
    return temp;
}

void* my_malloc(size_t size)
{
   //no memory allocated
    if(start_addr == NULL) 
    {
        //init useable memory
	if(init(size) == NULL) 
        {
            //no memory
            return NULL;
        }
    }
    //printf("is it new chunk?\n");
	//create new chunk_info
	chunk_info *freeBlock = findFirstFreeBlock(size); 
	//no free block of required size
	if(freeBlock == NULL) 
	{
		return NULL;
	}
	//if there is space to split block
	/*
	printf("--------------------\n");
	*/
	if(freeBlock->size > (size + CHUNK_SIZE))
	{
		//new block address = old block address + size to allocate + size of a chunk
		chunk_info *newBlock = ((void*)(((long unsigned int)freeBlock) + CHUNK_SIZE + ((long unsigned int)size)));
		/*
		printf("New block at: %lu\n",((long unsigned int)newBlock));
		*/
		//if block is not last black
		//printf("inside if? freeblock->size\n");
		if(freeBlock->next != NULL)
		{
			//new block size = to old block size - (size to allocate + size of a chunk)
			newBlock->size = freeBlock->size - (CHUNK_SIZE + size);
			//new block next points to old block next
			newBlock->next = freeBlock->next;
			//new block prev points to old block
			newBlock->prev = freeBlock;
			//new block is free
			newBlock->free = 1;

			//old block size = size to allocate
			freeBlock->size = size;
			//old block next points to new block
			freeBlock->next = newBlock;
		}
		//is last block
		else
		{
			freeBlock->size = size;
			
			newBlock->size = ((long unsigned int)end_addr) - ((long unsigned int)freeBlock + CHUNK_SIZE + freeBlock->size + CHUNK_SIZE);
			newBlock->next = NULL;
			newBlock->prev = freeBlock;
			newBlock->free = 1;
			
			freeBlock->next = newBlock;
		}
		/*
		printf("New block size = %lu\n", ((long unsigned int)newBlock->size));
		printf("New block next = %lu\n", ((long unsigned int)newBlock->next));
		printf("New block prev = %lu\n", ((long unsigned int)newBlock->prev));
		printf("New block free = %i\n",newBlock->free);
		printf("--------------------\n");
		*/
		//printHeap();
		//printf("malloc %i\n", ++count);
	}
	//old block is now not free
	freeBlock->free = 0;
	/*
	printf("Free block at = %lu\n",((long unsigned int)freeBlock));
	printf("Free block size = %lu\n", ((long unsigned int)freeBlock->size));
	printf("Free block next = %lu\n", ((long unsigned int)freeBlock->next));
	printf("Free block prev = %lu\n", ((long unsigned int)freeBlock->prev));
	printf("Free block free = %i\n", freeBlock->free);
	printf("--------------------\n");
	printf("Return address = %lu\n", (long unsigned int)freeBlock + CHUNK_SIZE);
	printf("--------------------\n");
	*/
	//return first address of old block chunk
	return ((void*)(((long unsigned int)freeBlock) + CHUNK_SIZE));
}

chunk_info* merge(chunk_info* b){
  /*  if (b == NULL )
    {
      printf("b = %p",b); 
    }

    if ( b->next == NULL )
    {
      printf("b->next = %p",b->next); 
      
    }

      if ( b->next->free == 0)
    {
      printf("b->next->free = %d",b->next->free); 
     
    }
  */
  if (b && b->next && b->next->free)
    {
      b->size += CHUNK_SIZE + b->next->size;
      b->next = b->next->next;
      if (b->next)
	{
	  b->next->prev = b;
	}
    }
  return b;
}

chunk_info* getBlock(void* ptr)
{
  char* tmp;
  tmp = ptr;

  return (ptr = tmp -= CHUNK_SIZE );
}
int valid_address(void* pointer)
{
  if(start_addr != NULL)
    {
      if(pointer > start_addr && pointer < sbrk(0))
	{
	  return (pointer == (getBlock(pointer))->ptr );
	}
    }
      return 0;
}

/*void my_free(void* ptr)
{
  // printf("---START OF FREE--- \n");
  chunk_info* tmp;
  
  if(valid_address(ptr))
    {
      tmp = getBlock(ptr);
      tmp->free = 1;
      

      if(tmp->prev && tmp->prev->free)
	{
	  //printf("tmp->prev->free = %d \n", tmp->prev->free);
	  tmp = merge(tmp->prev);
	}
      if(tmp->next)
	{
	  printf("tmp->next %p",tmp->next);
	  merge(tmp);
	}
      else
	{
	  if(tmp->prev)
	    {
	      tmp->prev->next = NULL;
	    }
	  else{
	    printf("END OF MEMEORY");
	  }
	}
    }
  //free_count++;
  //printf("free %i\n", free_count);
  //a  printf("---END OF FREE--- \n");
  } */

  //printHeap();

 void my_free(void* ptr)
{
  if(valid_address(ptr))
    { //pointer outside the range
      chunk_info* tmp = ((chunk_info*) (((size_t) ptr) - CHUNK_SIZE));
      if(tmp->next == ptr)
	{ //pointer not to a real chunk
	  tmp->free = 1; //free the pointer
	}
    }
}
