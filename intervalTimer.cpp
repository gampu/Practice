#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<cstdlib>
#include<cassert>
#include<unistd.h>
#include<string.h>
#include<sys/time.h>
#include<fcntl.h>
#include<sys/utsname.h>
#include<sys/ioctl.h>
#include<sys/inotify.h>
#include<sched.h>
#include<signal.h>
#include<thread>

/* Practice code to understand interval timer and signal related syscalls */

/* Environment pointer */
char **environ;

/* Error function */
void err( const char *str )
{
	std::cout << errno << "\n";
    perror( str );
    exit( 0 );
}

void sigAlarmHandler( int sig )
{
	std::cout << "Timer finished" << "\n"; /* UNSAFE */
}

int main( int argc, char *argv[] )
{
	/* Register signal handler for timer expire event */
	struct sigaction sa;
	sigset_t mask;
	sigemptyset( &mask );
	sa.sa_flags = 0;
	sa.sa_handler = sigAlarmHandler;
	if( sigaction( SIGALRM, &sa, NULL ) == -1 )
	{
		err( "sigaction" );
	}

	/* Create real(wall clock) timer */
	struct itimerval itv = { { 1, 0 }, { 5, 0 } };
	/* Timer expires after 5 seconds of creation and continues doing so with
       an interval of 1 second */
	if( setitimer( ITIMER_REAL, &itv, NULL ) == -1 )
	{
		err( "setitimer" );
	}
	/* Enable output for verifying whether the timer works */
	while( true )
	{
		/* Suspends execution until a signal is received */
		pause();
	}
    return 0;
}
