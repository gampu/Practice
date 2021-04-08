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
#include<linux/memfd.h>
#include<sys/syscall.h>
#include<sys/mman.h>

/* Practice to understand memfd_create. Doesn't work on 16.04.
   Works on 18.04 */

/* Environment pointer */
char **environ;

/* Error function */
void err( const char *str )
{
    std::cout << errno << "\n";
    perror( str );
    exit( 0 );
}

int main( int argc, char *argv[] )
{
    /* Create anonymous RAM file */
    int fd = memfd_create( "my_file", MFD_ALLOW_SEALING );
    if( fd == -1 )
    {
        err( "memfd_create" );
    }
    /* set its size to 100 bytes */
    if( ftruncate( fd, 100 ) == -1 )
    {
        err( "ftruncate" );
    }
    /* Output info for debugging */
    std::cout << getpid() << " " << fd << "\n";

    /* Set seals */
    if( fcntl( fd, F_ADD_SEALS, F_SEAL_GROW | F_SEAL_SHRINK | F_SEAL_WRITE |
                                F_SEAL_SEAL ) == -1 )
    {
        err( "fcntl" );
    }
    /* Get seals */
    int seals = fcntl( fd, F_GET_SEALS );
    if( seals == -1 )
    {
        err( "fcntl" );
    }
    if( seals & F_SEAL_GROW )
    {
        std::cout << "GROW" << "\n";
    }
    if( seals & F_SEAL_SHRINK )
    {
        std::cout << "SHRINK" << "\n";
    }
    if( seals & F_SEAL_WRITE )
    {
        std::cout << "WRITE" << "\n";
    }
    if( seals & F_SEAL_SEAL )
    {
        std::cout << "SEAL" << "\n";
    }
    /* Close file */
    if( close( fd ) == -1 )
    {
        err( "close" );
    }
    return 0;
}
