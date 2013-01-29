#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

// counts newline characters until zero terminator is found (end of string)
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
{ FILE* fp = NULL;
  char* buf = NULL;
  char* bufp;
  size_t bufsz, cursz, curpos;
  ssize_t ssz;
  struct stat st;
  int fd,numOfLines = 0;

  fp = popen("ls", "r");
  fd= fileno(fp);

  //uses fstat to determine size of the buffer from file descriptor provided
  if(fstat(fd, &st) >= 0) {
    bufsz = (size_t) st.st_blksize;
  } else {
    printf("error 1");
    exit(0);
  }

  // Allocate buffer of size BUFSZ.
  buf = (char *) malloc (bufsz);
  curpos = 0;
  cursz = bufsz;

  // Block read FD, storing data into BUF.
  while((ssz=read(fd, buf + curpos, bufsz)) > 0)
  { curpos+=ssz;
    cursz=curpos+bufsz;
    //if not enough memory for increasing the buffer, exit
    if(NULL == (bufp = (char *) realloc (buf, cursz)))
    { printf("error 2");
      exit(0);
    }
    buf = bufp;
  }

  // Zero-terminate BUF.
  buf[curpos] = '\0';
  pclose(fp);

  //count the number of lines in buf, which is the output of "ls"
  numOfLines = countLines(buf);
  if(numOfLines > 25)
  { //printf("too many lines");
    //open writable pipe to less editor and point fp to it
    fp = popen("less", "w");
    //print the whole of buf into less through the pointer
    fprintf(fp, "%s",buf);
    //close the pipe for less to be flushed into stdout
    pclose(fp);  
  } else  // 25 line or less
  { printf(buf);
  }

  return 0;
}
