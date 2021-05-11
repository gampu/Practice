/* C++ headers */
#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<cstdlib>
#include<cassert>
#include<utility>

/* C headers */
#include<unistd.h>
#include<string.h>
#include<sys/time.h>
#include<fcntl.h>
#include<sys/utsname.h>
#include<sys/ioctl.h>
#include<sys/inotify.h>
#include<sched.h>
#include<signal.h>
#include<pthread.h>

# define NUM_OF_THREADS 4

/* Environment pointer */
char** environ;

/* Error function */
void err( const char* str )
{
    std::cout << errno << "\n";
    perror( str );
    exit( 0 );
}

/* Global variables */
int produced = 0, consumed = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

/* Start function for a thread */
void* start( void* )
{
	for( int i = 1; i <= 50000; ++i )
	{
		/* Lock mutex for accessing num */
		if( pthread_mutex_lock( &mutex ) != 0 )
		{
			err( "pthread_mutex_lock" );
		}

		/* Access variables atomically and produce */
		++produced;
		std::cout << "Produced" << "\n";

		/* Unlock mutex after done */
		if( pthread_mutex_unlock( &mutex ) != 0 )
		{
			err( "pthread_mutex_unlock" );
		}

		/* Signal to waiting consumer */
		if( pthread_cond_signal( &cond ) != 0 )
		{
			err( "pthread_cond_signal" );
		}
	}
    return 0;
}

int main( int argc, char* argv[] )
{
	/* Create a thread */
	pthread_t thread_id;
 	if( pthread_create( &thread_id, NULL, start, NULL ) != 0 )
    {
    	err( "pthread_create" );
    }

	while( consumed < 50000 )
	{
		/* Wait for it to produce something */
		if( pthread_mutex_lock( &mutex ) != 0 )
		{
			err( "pthread_mutex_lock" );
		}

		while( produced == 0 )
		{
			if( pthread_cond_wait( &cond, &mutex ) != 0 )
			{
				err( "pthread_cond_wait" );
			}
		}

		/* Consume */
		while( produced > 0 )
		{
			--produced;
			++consumed;
			std::cout << "Consumed" << "\n";
		}

		/* Release the previous lock */
		if( pthread_mutex_unlock( &mutex ) != 0 )
		{
			err( "pthread_mutex_unlock" );
		}

		/* Debug information */
		std::cout << "Consumed " << consumed << " until now" << "\n";
	}
	if( pthread_join( thread_id, NULL ) != 0 )
	{
		err( "pthread_join" );
	}
	std::cout << "Produced: " << produced << " Consumed: " << consumed << "\n";
    return 0;
}
