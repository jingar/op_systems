#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

// counts newline characters until zero terminator is found (end of string)
int countLines(char* buf)
{ int i=0,counter=0;
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
{ FILE* fp = NULL;  //file pointer
  char* buf = NULL;  //buffer string
  char* bufp;  //buffer pointer
  size_t bufsz, cursz, curpos;  //buffer size, current total size of the buffer, current position in the buffer
  ssize_t ssz; //string size in the file
  struct stat st;  //structure for file properties, used by fstat
  int fd,numOfLines = 0;  //filedescriptor for fp conversion, line counter

  char* commandArgs = (char *) malloc (sizeof(char)*50); //allocates memeory for 50 characters for the command and args
  int i = 1;
  commandArgs = strcat(commandArgs, "ls ");
  while(i<argc)  //concats arguements with spaces after "ls"
  { commandArgs = strcat(commandArgs, argv[i]);
    commandArgs = strcat(commandArgs,  " ");
    i++;
  }
  fp = popen(commandArgs, "r"); //open pipe of ls command with read rights
  fd = fileno(fp); //convert FILE *fp into int fd

  //uses fstat to determine size of the buffer from file descriptor provided
  if(fstat(fd, &st) >= 0)
  { bufsz = (size_t) st.st_blksize;
  } else
  { printf("error 1\n");
    exit(0);
  }

  // Allocate buffer of size BUFSZ.
  buf = (char *) malloc (bufsz);
  curpos = 0;
  cursz = bufsz;

  // Block read FD, storing data into BUF.
  while((ssz=read(fd, buf + curpos, bufsz)) > 0) //read returns int>0, if successful
  { curpos+=ssz;
    cursz=curpos+bufsz;
    //if not enough memory for increasing the buffer, exit
    if(NULL == (bufp = (char *) realloc (buf, cursz)))
    { printf("error 2\n");
      exit(0);
    }
    buf = bufp;
  }

  // Zero-terminate BUF.
  buf[curpos] = '\0';
  pclose(fp);

  //count the number of lines in buf, which is the output of "ls"
  numOfLines = countLines(buf);
  if(numOfLines > 25)  printf("too many lines\n");
  // 25 line or less  
  else  printf("%s",buf);

  return 0;
}
