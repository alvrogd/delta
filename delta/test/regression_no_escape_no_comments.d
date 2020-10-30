import std.stdio;
import std.algorithm.iteration;
import std.exception;


double beta1(double[] x, double[] y, double x_bar, double y_bar)
{
    enforce(x.length==y.length, "x and y should be the same length!");
    double num=0, denom=0;
    int whilectr = 0;
    while (whilectr < x.length)
    {
        num += (x[whilectr] - x_bar) * (y[whilectr] - y_bar);
        whilectr++;
    }
    foreach (xval; x)
        denom += ((xval-x_bar)*(xval-x_bar));
    return num / denom;
}

void main()
{
    double[] x = [0b1, 2, 3, 4, 5, 6.5, 7, 7.5, 8_000/1.0e+03, 8.5];
    double[] y = [1, 2, 3, 3.2, 3.4, 40e-1, 4.2, 4.3, 4.35, 4.37];
    double x_bar, y_bar, b1, _0;
    x_bar = sum(x)/(cast(double)x.length);
    y_bar = sum(y)/(cast(double)y.length);
    b1 = beta1(x, y, x_bar, y_bar);
    _0 = y_bar - (b1*x_bar);
    a = 1;
}