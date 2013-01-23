#include <stdio.h>

int main(int argc, char** argv)
{   FILE *fp = NULL;
    char* buf = NULL;
    char* bufp;
    size_t bufsz, cursz, curpos;
    ssize-t ssz;
    struct stat st;

    *fp = popen(ls, 'r');

    if(fstat(*fp, &st) >= 0)
    	bufsz = (size_t) st.st_blksize;
    else
	printf("error 1");

    /*	Allocate buffer of size BUFSZ.	*/
    buf = (char *) malloc (bufsz);
    curpos =0;
    cursz = bufsz;

    /*	Block read FD, storing data into BUF.	*/
    while((ssz=read(*fp, buf + curpos, bufsz)) > 0)
    {	curpos+=ssz;
	cursz=curpos+bufsz;
	if(NULL == (bufp = (char *) realloc (buf, cursz)))
	    printf("error 2");
	buf = bufp;
    }

    /* Zero-terminate BUF. */
    buf[curpos] = 0;
