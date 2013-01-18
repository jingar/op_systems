#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>

void daemon_process(pid_t *pid,int *sid)
{
	
	/* Fork off the parent process */       
	*pid = fork();
	if (pid < 0) {
		exit(EXIT_FAILURE);
	}
	/* If we got a good PID, then
	we can exit the parent process. */
	if (pid > 0) {
		exit(EXIT_SUCCESS);
	}
	/* Change the file mode mask */
	umask(0);
	
	 /* Create a new SID for the child process */
       *sid = setsid();
        if (sid < 0) {
			/* Log any failure */
			exit(EXIT_FAILURE);
		}
		 /* Close out the standard file descriptors */
        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);
}


  int main(void)
  {
    time_t lt;
	FILE *out;
	pid_t pid;
	pid_t sid;
	
	/* Fork off the parent process */       
	pid = fork();
	if (pid < 0) {
		printf(" pid failure \n");
		exit(1);
	}
	/* If we got a good PID, then
	we can exit the parent process. */
	if (pid > 0) {
		printf(" pid success \n");
		exit(0);
	}
	/* Change the file mode mask */
	umask(0);
	
	 /* Create a new SID for the child process */
       sid = setsid();
        if (sid < 0) {
			printf(" sid failure \n");
			/* Log any failure */
			exit(EXIT_FAILURE);
		}
		 /* Close out the standard file descriptors */
        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);
	//daemon_process(&pid,&sid);
	out = fopen( "s_k_log.log", "a" );
    while(1)
	{
		if( out != NULL )
		{	
			lt = time(NULL);
			fprintf( out, "Time is %s\n", ctime(&lt) );
		}
		fflush(out);
		
	//openlog("Saad and Kos log",LOG_CONS | LOG_PID | LOG_NDELAY,0);
	
    //syslog(LOG_INFO,ctime(&lt));
    
    //closelog();
		sleep(10);
	}
    fclose(out);
    return 0;
  }
