#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

int countLines(char* buf)
{ int i,counter=0;
  char a;
  while((a=buf[i]) != '\0')
  { if(a=='\n')
    { counter++;  
    }
    i++;
  }
  return counter;
}

int main(int argc, char** argv)
{   FILE* fp = NULL;
    char* buf = NULL;
    char* bufp;
    size_t bufsz, cursz, curpos;
    ssize_t ssz;
    struct stat st;
    int fd,numOfLines = 0;


    fp = popen("ls", "r");
    fd= fileno(fp);

    if(fstat(fd, &st) >= 0) {
        bufsz = (size_t) st.st_blksize;
    } else {
	printf("error 1");
	exit(0);
    }

    //	Allocate buffer of size BUFSZ.
    buf = (char *) malloc (bufsz);
    curpos = 0;
    cursz = bufsz;

    //	Block read FD, storing data into BUF.
    while((ssz=read(fd, buf + curpos, bufsz)) > 0)
    {	curpos+=ssz;
	cursz=curpos+bufsz;
	if(NULL == (bufp = (char *) realloc (buf, cursz)))
	{ printf("error 2");
	  exit(0);
	}
	buf = bufp;
    }

    // Zero-terminate BUF.
    buf[curpos] = '\0';
    pclose(fp);

    numOfLines = countLines(buf);
    if(numOfLines > 25)
    { //printf("too many lines");
      fp = popen("less", "w");
      fprintf(fp, "%s",buf);
    }
    else
    { //printf(buf);
      fp = popen("less", "w");
      fprintf(fp, "%s",buf);
      pclose(fp);  
    }

    return 0;
}
