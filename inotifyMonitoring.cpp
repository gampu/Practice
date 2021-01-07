#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<cstdlib>
#include<cassert>
#include<unistd.h>
#include<string.h>
#include<sys/times.h>
#include<fcntl.h>
#include<sys/utsname.h>
#include<sys/ioctl.h>
#include<sys/inotify.h>

/* Practice code to understand inotify file monitoring syscalls */

/* Assuming max file size length is 256 */
#define BUF_SIZE 10 * ( sizeof( struct inotify_event ) + 256 + 1 )

/* Environment pointer */
char **environ;

/* Map to iterate over event mask */
std::vector< std::pair< int, std::string > > eventmap =
{
    { IN_ACCESS,        "IN_ACCESS" },
    { IN_ATTRIB,        "IN_ATTRIB" },
    { IN_CLOSE_NOWRITE, "IN_CLOSE_NOWRITE" },
    { IN_CLOSE_WRITE,   "IN_CLOSE_WRITE" },
    { IN_CREATE,        "IN_CREATE" },
    { IN_MODIFY,        "IN_MODIFY" },
    { IN_DELETE,        "IN_DELETE" },
    { IN_DELETE_SELF,   "IN_DELETE_SELF" },
    { IN_MOVE_SELF,     "IN_MOVE_SELF" },
    { IN_MOVED_FROM,    "IN_MOVED_FROM" },
    { IN_MOVED_TO,      "IN_MOVED_TO" },
    { IN_OPEN,          "IN_OPEN" },
};

/* Error function */
void err( const char *str )
{
    std::cout << errno << "\n";
    perror( str );
    exit( 0 );
}

/* Display's inotify event information */
void displayInotifyEvent( struct inotify_event *ptr )
{
    assert( ptr != NULL );
    /* Output wd and cookie fields */
    std::cout << ptr->wd << " " << ptr->cookie << "\n";
    /* Output file name if available */
    if( ptr->len )
    {
        std::cout << ptr->name << "\n";
    }
    /* Check for bits set in event mask */
    for( auto type: eventmap )
    {
        if( ptr->mask & type.first )
        {
            std::cout << type.second << "\n";
        }
    }
    std::cout << "Event has been output" << "\n";
}

int main( int argc, char *argv[] )
{
    /* Mild error handling */
    if( argc == 1 )
    {
        printf( "Usage: ./a.out file/dir name\n" );
        return 0;
    }
    /* Create an inotify instance */
    int fd = inotify_init();
    if( fd == -1 )
    {
        err( "inotify_init" );
    }
    std::cout << "Inotify instance created successfully" << "\n";
    /* Add watch items to the inotify instance for input files/directories */
    for( int i = 1; i <= argc - 1; ++i )
    {
        /* Add a single watch item */
        int wd = inotify_add_watch( fd, argv[ i ], IN_ALL_EVENTS );
        if( wd == -1 )
        {
            assert( close( fd ) != -1 );
            err( "inotify_add_watch" );
        }
        std::cout << "Added watch item for " << argv[ i ] << " with wd: "
                  << wd << "\n";
    }
    /* Allocate buffer */
    char buf[ BUF_SIZE ];
    /* Pointer to iterate */
    char *ptr = NULL;
    /* Pointer to event structure */
    struct inotify_event *event;
    while( true )
    {
        /* Read from fd using read */
        int num_bytes = read( fd, buf, BUF_SIZE );
        if( num_bytes == -1 )
        {
            close( fd );
            err( "read" );
        }
        /* Iterate over each inotify_event */
        ptr = buf; 
        while( ptr - buf < num_bytes )
        {
            /* Cast the pointer */
            event = ( struct inotify_event *)ptr;
            /* Output event */
            displayInotifyEvent( event );
            /* Advance the pointer */
            ptr += sizeof( struct inotify_event ) + event->len; 
        }
    }
    /* Will never reach this */
    return 0;
}
