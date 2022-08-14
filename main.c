#include <math.h>
#include <stdio.h>
#include "src/ffte.h"


int main()
{
    // Create data
    unsigned int N = 128;
    double x_r[N];
    double x_i[N];

    double f=9.375;//10 for N=100 , 9.375 for N=128
    double fs=200;

    for(int i=0; i< N; ++i)
    {
        x_r[i]=cos(2.0*M_PI*f/fs*((double)i));
    }
    
    // Print data
    printf("\nInput Signal:\n");
    for(int i=0; i< N; ++i)
    {
        printf("\nReal: %f",x_r[i]);
    }
    printf("\n");

    // Take FFT
    ffte(x_r,x_i,N,1);
    
    // Print output
    printf("\nFFT:\n");
    for(int i=0; i< N; ++i)
    {
        printf("\nf: %f, Abs: %f",i*fs/N, sqrt(x_r[i]*x_r[i] + x_i[i] * x_i[i]));
    }
    printf("\n");

    // Take IFFT
    iffte(x_r,x_i,N,0);
    
    // Print output
    printf("\nOutput Signal:\n");
    for(int i=0; i< N; ++i)
    {
        printf("\nReal: %f",x_r[i]);
    }
    printf("\n");

    return 0;
}

/*
main( int argc, char *argv[] )
{
    PLFLT x[NSIZE], y[NSIZE];
    PLFLT xmin = 0., xmax = 1., ymin = 0., ymax = 100.;
    int   i;

    // Prepare data to be plotted.
    for ( i = 0; i < NSIZE; i++ )
    {
        x[i] = (PLFLT) ( i ) / (PLFLT) ( NSIZE - 1 );
        y[i] = ymax * x[i] * x[i];
    }

    // Parse and process command line arguments
    plparseopts( &argc, argv, PL_PARSE_FULL );

    // Initialize plplot
    plinit();

    // Create a labelled box to hold the plot.
    plenv( xmin, xmax, ymin, ymax, 0, 0 );
    pllab( "x", "y=100 x#u2#d", "Simple PLplot demo of a 2D line plot" );

    // Plot the data that was prepared above.
    plline( NSIZE, x, y );

    // Close PLplot library
    plend();

    exit( 0 );
}
*/