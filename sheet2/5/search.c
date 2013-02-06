#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

typedef struct {char * file; char * pat;} input;
/* A function to searcg a substing in a file.  The file F has to be
   opened and the function is going to call subsequent fgetc on F
   until it reaches EOF or finds the match.

   Input arguments:
       F    -- input file opened for reading
       PAT  -- a pattern to be found.

   Return values:
       -1   -- in case the pattern was not found
      x > 0 -- where x is a position of the match.  */
int search (FILE *f, char *pat)
{
    int i, j, c, pos = 1;
    const char *ptr;

    for (; EOF != (c = fgetc (f)); pos++) {
        if (c != *pat)
            continue;

        for (i = 1, ptr = pat + 1; *ptr != '\0'; i++, ptr++)
            if (*ptr != (c = fgetc (f))) {
                ungetc (c, f);
                break;
            }

        if (*ptr == '\0')
            return pos;
        else
            for (j = i - 1; j > 0; j--)
                ungetc (pat[j], f);
    }

    return -1;
}

int search_wrapper(input* inst)
{ FILE * fp;
  if((fp = fopen(inst->file, "r")) == NULL) { perror("Bad input\n"); }
  pthread_exit(search(fp, inst->pat));
}

/* TODO: Check if the arguments are valid
   TODO: Todo, open the file safely
   TODO: Execute SEARCH for every file in ARGV
   TODO: Print the position found
   TODO: Make sure it does not segfaults in case
         arguments are wrong.

   XXX: Use a better searching algorihm.  */
int main (int argc, char *argv[])
{ if(argc<3)
  { puts("Please provide at least 2 arguements: a string pattern and a text file.\n");
    puts("For example: #test 1.txt\n");
    exit(EXIT_SUCCESS);
  }
  
  int i = 2;
  int pos[1] = {0};//,0,0,0,0,0,0,0,0,0};
  input * inst = malloc(sizeof(input));
  inst-> pat = argv[1];
  pthread_t* tids = malloc ((argc-2)*sizeof(pthread_t));


  while(i<argc)
  { inst-> file = argv[i];
    pthread_create(&tids[i-2], NULL, search_wrapper, (void *) inst);
    printf("CREATE: %d\n", i);
    i++;
  }
  i=2;
  while(i<argc)
  { pthread_join(tids[i-2], (void **)&pos);
    printf("JOIN: %d\n", i);
    if (-1 == (pos[0]))
    { printf ("pattern '%s' was not found in %s\n", inst->pat, argv[i]);
    } else
    { printf ("pattern '%s' found in %s offset=%d\n", inst->pat,argv[i],pos[0]);
    }
    i++;
  }
}
