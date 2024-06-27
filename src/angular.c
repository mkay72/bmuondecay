#include "angular.h"
#include <math.h>
#include <stdlib.h>

// j = abs(kappa) - 1/2
double jkappa(int kappa)
{
    return abs(kappa) - 0.5;
}

// abs(kappa + 1/2) - 1/2
int lkappa(int kappa)
{
    return (abs(2*kappa + 1) - 1)/2;
}

// abs(-kappa + 1/2) - 1/2
int lbarkappa(int kappa)
{
    return lkappa(-kappa);
}

// abs(kappa) = j + 1/2
int akappaj(double j)
{
    return (int)(j + 0.5);
}


int is_even(int n)
{
    return (n % 2 == 0) ? 1 : 0;
}

int is_odd(int n)
{
    return (n % 2 == 1) ? 1 : 0;
}

double pp(double j)
{
    return sqrt(2.0*j + 1);
}


int is_triangle(double j_a, double j_b, double j_c)
{
    int two_j_a = 2*j_a;
    int two_j_b = 2*j_b;
    int two_j_c = 2*j_c;
    if ((two_j_b < fabs(two_j_a - two_j_c)) || 
        (two_j_b > two_j_a + two_j_c) || is_odd(two_j_a + two_j_b + two_j_c))
    {
        return 0;
    }
    else
    {
        return 1;
    }
}


double cgc(double j1, double m1, double j2, double m2,  double j3, double m3)
{
    // double res;
    // res = ;
    double res = 0.0;
    hfd_cgc_(&j1, &m1, &j2, &m2, &j3, &m3, &res);
    return res;
}




double cgc_mzero(int l1, int l2, int l3)
{
    double res = 0.0;

    int gg = l1 + l2 + l3;
    int g = gg/2;

    if (is_even(gg))
    {
        double r1 = 0.0;
        double r2 = 0.0;
        r1 = lgamma(g + 1) - lgamma(g - l1 + 1) - lgamma(g - l2 + 1) - lgamma(g - l3 + 1);
        r2 = 0.5*(lgamma(gg - 2*l1 + 1) + lgamma(gg - 2*l2 + 1) + lgamma(gg - 2*l3 + 1) - lgamma(gg + 2));
        res = pow(-1.0, g - l3) * pp(l3) * exp(r1 + r2);
    }

    return res;
}





double ulbeta(int l, int kappa_a, int kappa_b, int lcapital, int beta)
{
    int l_a = lkappa(kappa_a);
    int l_b = lkappa(kappa_b);
    double res = 0.0;

    if (is_odd(l + l_a + l_b))
    {
        double b = (l == lcapital) ? 0.5 * sqrt(2) : 1.0;
        double a = (l == lcapital) ? 0.0 : 0.5 * (l * (l + 1) - lcapital * (lcapital + 1));
        double c = 0.0;

        if (l <= 0 && lcapital < 1)
        {
            c = 0.0;
        }
        else
        {
            c = pp(lcapital) * pow(0.5 * ((l + lcapital) * (l + lcapital + 1) * (l + lcapital + 2)) , -0.5);
        }

        res = c / b * (kappa_a + pow(-1.0, l - lcapital) * kappa_b + beta * a);
    }

    return res;
}


double slbeta(int l, int kappa_a, int kappa_b, int lcapital, int beta)
{
    int l_a = lkappa(kappa_a);
    int l_b = lkappa(kappa_b);
    double res = 0.0;

    if (is_even(l + l_a + l_b))
    {
        double b = (l == lcapital) ? 0.5 * sqrt(2) : 1.0;
        double a = (l == lcapital) ? 0.0 : 0.5 * (l * (l + 1) - lcapital * (lcapital + 1));
        double c = 0.0;

        if (l <= 0 && lcapital < 1)
        {
            c = 0.0;
        }
        else
        {
            c = pp(lcapital) * pow(0.5 * ((l + lcapital) * (l + lcapital + 1) * (l + lcapital + 2)) , -0.5);
        }

        res = c / b * (kappa_a + pow(-1.0, l - lcapital + 1) * kappa_b + beta * a);
    }

    return res;
}
