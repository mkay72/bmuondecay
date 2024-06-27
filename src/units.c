#include "units.h"

double MeV_to_RU(double ru)
{
    return ru / me_MeV;
}

double MeV_to_gau(double e_MeV, double m)
{
    return e_MeV * 1e6 / E_HARTREE_EV / m;
}

double keV_to_gau(double e_keV, double m)
{
    return e_keV * 1e3 / E_HARTREE_EV / m;
}


double fm_to_gau(double fm, double m)
{
    return fm * 1e-15 * m / A_BOHR_E_SI;
}

double gau_to_fm(double gau, double m)
{
    return gau * A_BOHR_E_SI / 1e-15 / m;
}

double length_gau1_to_gau2(double x, double m1, double m2)
{
    return x * m2 / m1;
}

double energy_gau1_to_gau2(double e, double m1, double m2)
{
    return e * m1 / m2;
}