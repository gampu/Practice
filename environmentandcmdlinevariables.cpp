#include<iostream>
using namespace std;

/* Practice code to understand environment variables */

extern char **environ;

int main( int argc, char *argv[] )
{
    /* Output environment variables */
    for( char **env_ptr = environ; *env_ptr != NULL; ++env_ptr )
    {
        puts( *env_ptr );
    }

    /* Output command line arguments */
    for( int i = 1; i <= argc; ++i )
    {
        puts( *( argv + i - 1 ) ); /* Or puts( argv[ i - 1 ] ); */                                 
    }
    return 0;
}
