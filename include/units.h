#ifndef UNITS_H_INCLUDED
#define UNITS_H_INCLUDED

#include "constants.h"

double MeV_to_RU(double ru);

double MeV_to_gau(double e_MeV, double m);

double keV_to_gau(double e_keV, double m);

double fm_to_gau(double fm, double m);

double gau_to_fm(double gau, double m);

double length_gau1_to_gau2(double x, double m1, double m2);

double energy_gau1_to_gau2(double e, double m1, double m2);

#endif