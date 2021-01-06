#include<iostream>
#include<cstdarg>
#include<string>

/* Practice code to undertand variable list arguments */

int sum( int n, std::string s, ... )
{
    int sum = 0;
    va_list ap;
	/* Init ap after the parameter s */
    va_start( ap, s );
    for( int i = 1; i <= n; ++i )
    {
		/* Fetch int varible */
        sum += va_arg( ap, int );
    }
    va_end( ap );
    return sum;
}

int main( void )
{
	/* Calculates sum of the last 3 arguments */
    std::cout << sum( 3, "gampu", 10, 20, 30 ) << "\n";
    return 0;
}
