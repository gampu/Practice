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

/* Fibonacci function */
int solve( int n )
{
    if( n <= 3 )
    {
        return n;
    }
    return solve( n - 1 ) + solve( n - 2 );
}

/* Start function for a thread */
void* start( void* arg )
{
    /* Calculate fibonacci value for the first variable and store the answer
       in the second variable */
    std::pair< int, int >* p = ( std::pair< int, int >* )arg;
    p->second = solve( p->first );
    return 0;
}

int main( int argc, char* argv[] )
{
    pthread_t threads[ NUM_OF_THREADS ];
    std::pair< int, int > arr[ NUM_OF_THREADS ] = { { 45, 0 }, { 45, 0 },
                                                    { 45, 0 }, { 45, 0 }
                                                  };

    #if THREADED
    /* Use -DTHREADED to include the below code in the executable */ 
    for( int i = 0; i <= NUM_OF_THREADS - 1; ++i )
    {
        if( pthread_create( &threads[ i ], NULL, start,
            ( void *)&arr[ i ] ) != 0 )
        {
            err( "pthread_create" );
        }
    }
    for( int i = 0; i <= NUM_OF_THREADS - 1; ++i )
    {
        if( pthread_join( threads[ i ], NULL ) != 0 )
        {
            err( "pthread_join" );
        }
        std::cout << "Fibonacci of " << arr[ i ].first << ": " <<
                     arr[ i ].second << "\n";
    }
    #endif
    #if NOTTHREADED 
    /* Use DNOTTHREADED to include the below code in the executable */
    for( int i = 0; i <= NUM_OF_THREADS - 1; ++i )
    {
        std::cout << "Fibonacci of " << arr[ i ].first << ": " <<
                     solve( arr[ i ].first ) << "\n";
    }
    #endif
    return 0;
}
