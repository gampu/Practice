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
#include<sys/types.h>
#include<sched.h>
#include<signal.h>
#include<pthread.h>
#include<dirent.h>

# define NUM_OF_THREADS 3

/* Environment pointer */
char** environ;

/* Error function */
void err( const char* str )
{
    std::cout << errno << "\n";
    perror( str );
    exit( 0 );
}

int cnt = 0;

int main( int argc, char* argv[] )
{
	/* Get current dir.*/
    char* p = get_current_dir_name();
	if( !p )
	{
		err( "get_current_dir_name" );
	}
	printf( "Current dir: %s\n", p );
	/* Open directory */
	DIR* d = opendir( p );
	if( !d )
	{
		/* Free pointer p before exiting */
		free( p );
		err( "opendir" );
	}
	/* Iterate over the directory */
	struct dirent* it = 0;
	while( ( it = readdir( d ) ) != 0 )
	{
		/* Ignore . && .. directories */
		if( it->d_name[ 0 ] != '.' )
		{
			++cnt;
			printf( "%s\n", it->d_name );
		}
	}
	printf( "Total: %d\n", cnt );
	/* Close dir */
	assert( closedir( d ) == 0 );

	/* Free pointer p */	
	free( p );
    return 0;
}
