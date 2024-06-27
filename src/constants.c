#include "constants.h"

const double ONE_OVER_PI4 = 0.0795774715459477;
const double ONE_OVER_PI2 = 0.1591549430918953;
const double ONE_OVER_PI = 0.3183098861837907;
const double PI = 3.141592653589793;

const double complex ZONE = CMPLX(1.0, 0.0);
const double complex ZZERO = CMPLX(0.0, 0.0);
const double complex ZONE_OVER_TWO = CMPLX(0.5, 0.0);


// sqrt(pi)
const double SQRT_PI = 1.772453850905516;
// sqrt(pi/2)
const double SQRT_PI_OVER_2 = 1.253314137315500;
// pi/2
const double PI_OVER_2 = 1.570796326794897;
// 1/sqrt(pi)
const double ONE_OVER_SQRT_PI = 0.5641895835477563;

// CODATA 2018
const double ALPHA = 0.0072973525693;
const double SPEED_OF_LIGHT = 137.035999084;

// const char ASTR = '*';
// const char COMM = ',';


// electron mass [MeV]
const double me_MeV = 0.510998950;
// muon mass in units if me
const double mmuon = 206.7682830;
// reduced mass of two protons in units of me
const double mup = 0.5 * 938.27208816 / me_MeV;
// reduced mass of the deuteron in units of me
const double mud = (938.27208816 * 939.56542052) / (938.27208816 + 939.56542052) / me_MeV;

const double E_HARTREE_EV = 27.211386245988;
const double A_BOHR_E_SI = 5.29177210903 * 1e-11;

const double E_R_CLASSICAL_FM = 2.8179403262;