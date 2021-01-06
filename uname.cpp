#include<iostream>
#include<cstdlib>
#include<cassert>
#include<unistd.h>
#include<string.h>
#include<sys/times.h>
#include<fcntl.h>
#include<sys/utsname.h>

/* Practice code to understand uname syscall */

using namespace std;

/* Error function */
void err( const char *str )
{
    perror( str );
    exit( 0 );
}

int main( int argc, char *argv[], char *envp[] )
{
    struct utsname buf;
    if( uname( &buf ) == -1 )
    {
        err( "uname" );
    }
    printf( "%s\n%s\n%s\n%s\n%s\n%s\n", buf.sysname, buf.nodename, buf.release,
            buf.version, buf.machine, buf.domainname );
    return 0;
}
