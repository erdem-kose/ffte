#include <math.h>
#include <stdio.h>
#include <plplot/plplot.h>
#include "src/ffte.h"

void plot(const char* filename, double x[], double y[], int N, const char* x_lbl, const char* y_lbl)
{
    PLFLT xmin = INFINITY, xmax = -INFINITY;
    PLFLT ymin = INFINITY, ymax = -INFINITY;

    unsigned int i;
    for(i=0; i<N; ++i)
    {
        xmax=(x[i]>xmax)?x[i]:xmax;

        if(x[i]<xmin)
            xmin=x[i];
        if(y[i]>ymax)
            ymax=y[i];
        if(y[i]<ymin)
            ymin=y[i];
    }
    // Plplot
    plsetopt("dev","pdf");
    plsetopt("o", filename);
    
    plinit();
    
    plcol0( 1 );
    plenv( xmin, xmax, ymin, ymax, 0, 0 );

    plcol0( 6 );
    pllab( x_lbl, y_lbl, "" );

    plcol0( 4 );
    plline( N, x, y );

    plend();
}

void test_ffte1d()
{
    // Create data
    unsigned int N = 128;
    double x_r[N];
    double x_i[N];
    double f_axis[N];
    double t_axis[N];
    double f=9.375;//10 for N=100 , 9.375 for N=128
    double fs=200;

    for(int i=0; i< N; ++i)
    {
        x_r[i]=cos(2.0*M_PI*f/fs*((double)i));
        f_axis[i]=i*fs/N;
        t_axis[i]=i/fs;
    }

    // Plot data
    plot("../ffte_x.pdf", t_axis, x_r , N, "t", "X");

    // Take FFT
    ffte(x_r,x_i,N,1);

    plot("../ffte_Xr.pdf", f_axis, x_r , N, "f", "X");
    plot("../ffte_Xi.pdf", f_axis, x_i , N, "f", "X");

    // Take IFFT
    iffte(x_r,x_i,N,0);

    plot("../ffte_x_ifft.pdf", t_axis, x_r , N, "t", "X");
}

int main(int argc, char **argv )
{
    test_ffte1d();

    return 0;
}