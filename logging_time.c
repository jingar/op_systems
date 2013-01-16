#include <time.h>
#include <stdio.h>
#include <syslog.h>
  int main(void)
  {
    time_t lt;
	FILE *out;
    
    while(1)
	{
		out = fopen( "s_k_log.log", "a" );
		if( out != NULL )
		{	
			lt = time(NULL);
			fprintf( out, "Time is %s\n", ctime(&lt) );
		}
		fclose(out);
		
	//openlog("Saad and Kos log",LOG_CONS | LOG_PID | LOG_NDELAY,0);
	
    //syslog(LOG_INFO,ctime(&lt));
    
    //closelog();
		sleep(10);
	}
    
    return 0;
  }
