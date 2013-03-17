#include <time.h>
#include <stdio.h>
#include <syslog.h>
int main(void)
{ time_t lt;
  FILE *out;
    
  while(1)  //infinite loop
  { out = fopen( "time.log", "a" );
    //out is a file pointer with append rights
    if( out != NULL ) //if file exists
    { lt = time(NULL); //current time
      fprintf( out, "Time is %s\n", ctime(&lt) ); //print into the log
    }
    fclose(out); //close file
    //openlog("Saad and Kos log",LOG_CONS | LOG_PID | LOG_NDELAY,0);
    //syslog(LOG_INFO,ctime(&lt));  
    //closelog();
    sleep(10); //wait for 10 seconds
  }
  return 0;
}
