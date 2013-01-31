#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* A function to searcg a substing in a file.  The file F has to be
   opened and the function is going to call subsequent fgetc on F
   until it reaches EOF or finds the match.

   Input arguments:
       F    -- input file opened for reading
       PAT  -- a pattern to be found.

   Return values:
       -1   -- in case the pattern was not found
      x > 0 -- where x is a position of the match.  */
int search (FILE *f, const char *pat)
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
  int pos = 0;
  FILE * fp;
  const char* patt = argv[1];

  while(i<argc)
  { if((fp = fopen(argv[i], "r")) == NULL) { perror("Bad input\n"); }
    if (-1 == (pos=search(fp, patt)))
    { printf ("pattern '%s' was not found in %s\n", patt, argv[i]);
    } else
    { printf ("pattern '%s' found in %s offset=%d\n", patt,argv[i],pos);
    }
    i++;
  }
}
