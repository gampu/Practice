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

/* Practice code to understand gettimeofday syscall with OOP design */

/* Environment pointer */
char **environ;

/* Error function */
void err( const char *str )
{
	std::cout << errno << "\n";
    perror( str );
    exit( 0 );
}

/* Time Class */
class Time
{
public:
	/* Structure to store time at instantiation*/
	struct timeval t;

	Time( void )
	{
		/* Get current time and store it in t */
		if( gettimeofday( &t, NULL ) != 0 )
		{
			/* If problem occurs, throw runtime error */
			throw std::runtime_error( "gettimeofday" );
		}
	}
	
	/* A friendly member function to output the timeval struct */
	friend std::ostream& operator<<( std::ostream& out, const Time& T )
	{
		out << T.t.tv_sec << " " << T.t.tv_usec << "\n";
		return out;
	}
};

int main( int argc, char *argv[] )
{
	/* Create a time object and output it */
	Time t;
	std::cout << t << "\n";
    return 0;
}
