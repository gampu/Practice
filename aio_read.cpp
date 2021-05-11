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
#include<aio.h>

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

char buf[ 512 ] = { 0 };

void let_me_know( sigval_t sigval )
{
	/* Get the aio context back */
	struct aiocb* my_aiocb = ( struct aiocb* )sigval.sival_ptr;
	
	/* Get status info */
	if( aio_error( my_aiocb ) == 0 )
	{
		/* Get read info */
		int bytes_read;
		if( ( bytes_read = aio_return( my_aiocb ) ) != -1 )
		{
			std::cout << bytes_read << "\n";
			printf( "%s\n%lu\n", buf, strlen( buf ) );
		}
		else
		{
			/* Read error */
			std::cout << "Read error occurred." << "\n";
		}
	}
	else
	{
		/* Aio request related error */
		std::cout << "Request error occurred." << "\n";
	}
}
	
int main( int argc, char* argv[] )
{
	/* Open a file */
	int fd = open( "sicp.pdf", O_RDONLY );
	if( fd == -1 )
	{
		err( "fd" );
	}

	/* Create a aio context */
	struct aiocb my_aiocb;

	/* Zero it out */
	memset( &my_aiocb, 0, sizeof( struct aiocb ) );

	/* Fill in the details */
	my_aiocb.aio_fildes = fd;
	my_aiocb.aio_buf = &buf;
	my_aiocb.aio_nbytes = 1000;
	my_aiocb.aio_sigevent.sigev_notify = SIGEV_THREAD;
	my_aiocb.aio_sigevent.sigev_notify_function = let_me_know;
	my_aiocb.aio_sigevent.sigev_value.sival_ptr = &my_aiocb;

	/* Enqueue a read request */
	if( aio_read( &my_aiocb ) != 0 )
	{
		close( fd );
		err( "aio_read" );
	}

	/* Wait */
	while( true )
	{
		/* Do nothing */
	}
    return 0;
}
