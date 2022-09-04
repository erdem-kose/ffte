#include <math.h>
#include <plplot/plplot.h>
#include "plot.h"

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