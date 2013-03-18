#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void kmpsearch(char *pattern, char *txt)
{
  printf("kmp search start");
  int pattern_length = strlen(pattern);
  int txt_length = strlen(txt);
  int j = 0;

  int *table = malloc(sizeof(int) * pattern_length);
  if(table == NULL)
    {
      printf("Out of memory \n");
    }

  kmptable(pattern,pattern_length,table);

  int i = 0;
  while(i < txt_length)
    {
      printf("kmp search while :: pattern[i] =  %d and txt[j] = %d", i,j); 
      if(pattern[j] == txt[i])
	{
	  printf("inside first if");
	  j++;
	  i++;
	}
      if (j ==  pattern_length)
	{
	  printf("Found pattern at index %d \n", i-j);
	  j = table[j-1];
	}
      else if(pattern[j] != txt[i])
	{
	  if(j != 0)
	    {
	      j = table[j-1];
	    }
	  else{
	    i= i + 1;
	  }
	}
    }
  free(table);
}

void kmptable(char *pattern, int pattern_size, int *table)
{
  printf("kmptable start");
  int i = 1;
  int j = 0;
  table[0] = 0;
  while(i < pattern_size)
    {
      if(pattern[i] == pattern[j])
	{
	  j++;
	  table[i] = j;
	  i++;
	}
      else{
	if(j != 0)
	  {
	    j = table[j-1];
	  }
	else{
	    table[i] = 0;
	    i++;
	}
      }
    }
}


int main(int argc, char *agv[])
{
  char *txt = "ABABDABACDABABCABAB";
  char *pat = "ABABCABAB";
  printf("Main before kmpsearch \n");
  kmpsearch(pat,txt);
  exit(0);
}
