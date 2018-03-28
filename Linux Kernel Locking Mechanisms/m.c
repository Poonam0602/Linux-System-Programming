#define _GNU_SOURCE //tells compiler that we are using GNU. -> to avoid warnings for CPU affinity set
#include <sys/types.h>
#include <unistd.h>
#include<stdlib.h>
#include <fcntl.h>
#include <stdio.h>
/* setpriority*/
#include <sys/resource.h>
#include <sys/time.h>
#include <sched.h> 
#include <signal.h>
#include <wait.h>
#include<time.h>


cpu_set_t mask;
int main () 
{
    // Set CPU affinity
    CPU_ZERO( &mask );
    CPU_SET( 1, &mask );
    sched_setaffinity(0, sizeof(mask), &mask);
    struct sched_param param;
    int begin,end,terminate;
    double elapsed;
    char enter = 0;
    terminate = 1;
    param.sched_priority = 3;
    if( sched_setscheduler( 0, SCHED_FIFO, &param ) == -1 ) 
    {
        fprintf(stderr,"error setting scheduler ... are you root?\n");
        exit(1);
    }

    param.sched_priority = 3;
    if( sched_setparam( 0, &param ) == -1 ) 
    {
        fprintf(stderr,"Error setting priority!\n");
        exit(1);
    }
    printf("Middle Priority = %d\t  PID = %d",param.sched_priority,getpid());
    printf("\n Press enter to continue:");
    
   
    while (enter != '\n') 
        enter = getchar();
    printf("\n Middle starts");
    begin= time(NULL);
    while(terminate)
	{
        end = time(NULL);
        elapsed = difftime(end, begin);
        if (elapsed >= 30.0 /* seconds */)
           terminate = 0;
        else  // No need to sleep when 90.0 seconds elapsed.
            usleep(50000);
        
	}
	end = time(NULL);
	printf("\n Start time = %d",begin);
	printf("\n End time = %d\n",end);
    return 0;
}
    
