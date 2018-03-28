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
#include <time.h>

int main () 
{
    struct sched_param param;
    int begin,end;

    int fd;
    char bufw[80];
    fd = open("mydevice", O_RDWR);
    perror ("  ");

    param.sched_priority = 2;
    if( sched_setscheduler( 0, SCHED_FIFO, &param ) == -1 ) 
    {
        fprintf(stderr,"error setting scheduler ... are you root?\n");
        exit(1);
    }

    param.sched_priority = 2;
    if( sched_setparam( 0, &param ) == -1 ) 
    {
        fprintf(stderr,"Error setting priority!\n");
        exit(1);
    }
    printf("Low Priority = %d\t     PID = %d\n",param.sched_priority,getpid());
    printf ("Enter to device: ");
    scanf ("%s", bufw);
    begin= time(NULL);
    write(fd, bufw, sizeof(bufw));
    end = time(NULL);
    printf("\n Start time : %d",begin);
    printf("\n End time : %d\n",end);
    close(fd);
    return 0;
}
    
