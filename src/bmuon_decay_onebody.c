#include <math.h>

#include "qgrid.h"
#include "fqgrid.h"
#include "fqgrid_stack.h"
#include "error_handler.h"

#include "multptws.h"
#include "contr.h"

#include "constants.h"
#include "units.h"
#include "angular.h"

#include "sphbes.h"
#include "name.h"
#include "timer.h"
#include "libradial_interface.h"
#include "options.h"



void pretabulate_gauss_legendre(void);


extern double* gl_table[2][65];

static double e_muon;

static double partial_transrate_mu_decay(int znucl, int n_muon, int kappa_muon, double e_e, int akappa_e)
{

    int l_muon = lkappa(kappa_muon);
    double j_muon = jkappa(kappa_muon);

    name_t* tname_psi_mu = new_name("psi_mu(x)");
    name_t* tname_psi_e = new_name("psi_e(x)");
    name_t* tname_jl = new_name("j(k*x)");


    int inuclear_model = options->inuclear_model;
    double rms_fm = options->rms;


    double k0 = e_muon - e_e;



    double transrate_tot = 0.0;
    double transrate_vsvs_tot = 0.0;
    double transrate_vsvv_tot = 0.0;
    double transrate_vvvv_tot = 0.0;


    double complex r1sum;
    double complex r2sum;

    int lcapital_max = 53;
    double r1[2][lcapital_max];
    double r2[2][lcapital_max];

    for (int i=0; i < lcapital_max; i++)
    {
        r1[0][i] = 0.0; r1[1][i] = 0.0;
        r2[0][i] = 0.0; r2[1][i] = 0.0;
    } 



    int nsub_k = options->nsub_k;
    int nqnode_k = options->nqnode_k;
    qgrid_t* qgrid_k = new_qgrid((double[]){0, k0}, 2, nsub_k, nqnode_k, "kk");            


    fqgrid_stack_pos_t pos_akappa = get_stack_pos();

    for (int skappa_e = -1; skappa_e <= +1; skappa_e += 2)
    {
        
        int kappa_e = skappa_e * akappa_e;
        int l_e = lkappa(kappa_e);
        double j_e = jkappa(kappa_e);

        int lcapital_min = (int) fabs(j_muon - j_e);
        int lcapital_max = (int) (j_muon + j_e);

        fqgrid_stack_pos_t pos_skappa = get_stack_pos();

        timer_start("psi_e(x)");
        double phase = eval_dfree_radial(znucl, rms_fm, inuclear_model, 1.0, e_e-1.0, kappa_e, tname_psi_e->fullname);
        timer_stop("psi_e(x)");

        double transrate_kappa = 0.0;
        double transrate_vsvs_kappa = 0.0;
        double transrate_vsvv_kappa = 0.0;
        double transrate_vvvv_kappa = 0.0;

        for (int ik = 0; ik < qgrid_k->n; ik++)
        {
            double k = qgrid_k->x[ik];
            double weight_k = qgrid_k->w[ik];
            
            double i1 = 0.0;
            double i2 = 0.0;
            double i3 = 0.0;
            double i4 = 0.0;

            fqgrid_stack_pos_t pos_k = get_stack_pos();

            for (int lcapital = lcapital_min; lcapital <= lcapital_max; lcapital++)
            {
                int ilcapital = (lcapital == 0) ? lcapital - lcapital_min : lcapital - lcapital_min + 1;

                int lmin = fmax(0, lcapital - 1);
                int lmax = lcapital + 1;
                int nl = lmax - lmin + 1;

                double integral_type1_k = 0.0;
                double integral_type2_k = 0.0;
                double integral_type3_k = 0.0;
                double integral_type4_k = 0.0;         

                double angular_lcapital = pow(pp(j_e) * cgc(j_e, 0.5, (double)lcapital, 0.0, j_muon, 0.5), 2.0);


                // evaluation of the radial R1_L(k) and R2_L(k) integrals
                for (int l = lmin; l <= lmax; l++)
                {
                    modify_suffix(tname_jl, "l%d", l);

                    timer_start("jl(k*x)");
                    if (find_fqgrid_stack(tname_jl->fullname) == NULL)
                    {
                        eval_sphbes_amos(SPHBES_J, (double)l, (double complex)k, tname_jl->fullname);
                    }
                    timer_stop("jl(k*x)");


                    timer_start("radint");
                    for (int beta = 1; beta >= -1; beta -= 2)
                    {
                        // beta = +1 -> ibeta = 0
                        // beta = -1 -> ibeta = 1
                        int ibeta = abs(beta - 1)/2;

                        modify_suffix(tname_psi_e, "[%+d]", beta);
                        modify_suffix(tname_psi_mu, "[%+d]", beta);

                        r1[ibeta][l] = creal(contr_vdiaggv(tname_psi_mu->fullname, NO_CONJ, tname_jl->fullname, tname_psi_e->fullname));

                        modify_suffix(tname_psi_mu, "[%+d]", -beta);

                        r2[ibeta][l] = creal(contr_vdiaggv(tname_psi_mu->fullname, NO_CONJ, tname_jl->fullname, tname_psi_e->fullname));

                        modify_suffix(tname_psi_e, "");
                        modify_suffix(tname_psi_mu, "");
                    }
                    timer_stop("radint");
                }

                // evaluation of the integrals I1_L(k), I2_L(k), I3_L(k), I4_L(k)
                // integral_type1_k
                r1sum = 0.0;
                r2sum = 0.0;
                for (int beta = 1; beta >= -1; beta -= 2)
                {
                    int ibeta = abs(beta - 1)/2;
                    r1sum += r1[ibeta][lcapital];
                    r2sum += beta * r2[ibeta][lcapital];
                }

                integral_type1_k = pow(pp(lcapital), 2.0) * 
                                (is_even(l_e + l_muon + lcapital) * r1sum * r1sum +
                                is_odd(l_e + l_muon + lcapital) * r2sum * r2sum);


                //integral_type2_k
                for (int l = lmin; l <= lmax; l++)
                {
                    r1sum = 0.0;
                    r2sum = 0.0;

                    for (int beta = 1; beta >= -1; beta -= 2)
                    {
                        int ibeta = abs(beta - 1)/2;

                        r1sum += beta * slbeta(l, kappa_e, kappa_muon, lcapital, beta) * r1[ibeta][l] - 
                                    I * ulbeta(l, kappa_e, kappa_muon, lcapital, beta) * r2[ibeta][l];
                        r2sum += is_even(l_e + l_muon + lcapital) * r1[ibeta][lcapital] + 
                                is_odd(l_e + l_muon + lcapital) * I * beta * r2[ibeta][lcapital];                            
                    }

                    integral_type2_k += -2.0*creal(cpow(I, lcapital - l) * pp(l) * pp(lcapital) * cgc_mzero(lcapital, 1, l) * r1sum * r2sum);
                }


                // integral_type3_k
                for (int l = lmin; l <= lmax; l++)
                {
                    r1sum = 0.0;
                    r2sum = 0.0;

                    for (int beta = 1; beta >= -1; beta -= 2)
                    {
                        int ibeta = abs(beta - 1)/2;

                        r1sum += ulbeta(l, kappa_e, kappa_muon, lcapital, beta) * r2[ibeta][l];
                        r2sum += beta * slbeta(l, kappa_e, kappa_muon, lcapital, beta) * r1[ibeta][l];

                    }

                    integral_type3_k += pow(pp(l), 2.0) * (r1sum*r1sum + r2sum*r2sum);
                }


                // integral_type4_k
                for (int l1 = lmin; l1 <= lmax; l1++)
                {
                    for (int l2 = lmin; l2 <= lmax; l2++)
                    {
                        int il2 = l2 - lmin;

                        r1sum = 0.0;
                        r2sum = 0.0;

                        if (is_even(l1 + l2))
                        {
            
                            for (int beta = 1; beta >= -1; beta -= 2)
                            {
                                int ibeta = abs(beta - 1)/2;

                                r1sum += I * ulbeta(l1, kappa_e, kappa_muon, lcapital, beta) * r2[ibeta][l1] - 
                                        beta * slbeta(l1, kappa_e, kappa_muon, lcapital, beta) * r1[ibeta][l1];
                                r2sum += -I * ulbeta(l2, kappa_e, kappa_muon, lcapital, beta) * r2[ibeta][l2] - 
                                        beta * slbeta(l2, kappa_e, kappa_muon, lcapital, beta) * r1[ibeta][l2];                                    
                            }
                            integral_type4_k += cpow(I, -l1 + l2) * pp(l1) * pp(l2) * 
                                                cgc_mzero(lcapital, 1, l1) * cgc_mzero(lcapital, 1, l2) * 
                                                r1sum * r2sum;
                        }
                    }
                }


                i1 += angular_lcapital * integral_type1_k;
                i2 += angular_lcapital * integral_type2_k;
                i3 += angular_lcapital * integral_type3_k;
                i4 += angular_lcapital * integral_type4_k;
            }

            transrate_vsvs_kappa += weight_k*(8.0 / pow(mmuon, 5.0) * k*k * (k*k * i1));
            transrate_vsvv_kappa += weight_k*(8.0 / pow(mmuon, 5.0) * k*k * (k0*k * i2));
            transrate_vvvv_kappa += weight_k*(8.0 / pow(mmuon, 5.0) * k*k * (k*k * i4 + (k0*k0 - k*k)*i3));

            transrate_kappa += (transrate_vsvs_kappa + transrate_vsvv_kappa + transrate_vvvv_kappa);

            restore_stack_pos(pos_k);
        }

        transrate_vsvs_tot += transrate_vsvs_kappa;
        transrate_vsvv_tot += transrate_vsvv_kappa;
        transrate_vvvv_tot += transrate_vvvv_kappa;

        transrate_tot = transrate_vsvs_tot + transrate_vsvv_tot + transrate_vvvv_tot;

        restore_stack_pos(pos_skappa);
    }
    restore_stack_pos(pos_akappa);

    delete_qgrid(qgrid_k);

    delete_name(tname_jl);
    delete_name(tname_psi_mu);
    delete_name(tname_psi_e);


    return transrate_tot;

}






int bmuon_decay_onebody(void)
{

    pretabulate_gauss_legendre();

    double a = options->r1;
    double b = options->r2;
  
    int nsub_1 = options->nsub_r;
    int nqnode_1 = options->nqnode_r;

    int znucl = options->znucl;
    int n_muon = options->n_muon;
    int kappa_muon = options->kappa_muon;

    double rms_fm = options->rms;
    int inuclear_model = options->inuclear_model;

    int akappa_e_min = options->akappa_e_min;
    int akappa_e_max = options->akappa_e_max;

    double e_e_min = options->energy_e_min;
    double e_e_max = options->energy_e_max;
    

    qgrid_t* qgrid_1 = new_qgrid((double[]){a, b}, 2, nsub_1, nqnode_1, "x");
    qgrid_stack_push(qgrid_1);

    e_muon = eval_dbound_radial(znucl, rms_fm, inuclear_model, mmuon, n_muon, kappa_muon, "psi_mu(x)");
   

    
    int nsub_e = options->nsub_e;
    int nqnode_e = options->nqnode_e;
    qgrid_t* qgrid_e = new_qgrid((double[]){e_e_min, e_e_max}, 2, nsub_e, nqnode_e, "ee");


    fqgrid_stack_pos_t pos_start = get_stack_pos();


    timer_new_entry("tot", "total time");
    timer_new_entry("jl(k*x)", "evaluation of j(k*x)");
    timer_new_entry("psi_e(x)", "evaluation of psi_e(x)");
    timer_new_entry("radint", "radial integrals R1_L(k) and R2_L(k)");
    timer_start("tot");


    printf("\n\n\t**********************************\n");
    printf("\t** Beginning of the calculation **\n");
    printf("\t**********************************\n");

//     if (options->print_level > 0)
//     {
        printf("%-16s = %12.8f [MeV]\n", "e_muon", e_muon * me_MeV);
        printf("%-16s = %12.8f [MeV]\n", "e_muon_bind", (mmuon - e_muon )* me_MeV);
//     }
    
    if (options->print_level == 0)
    {
        printf("%16s\t%16s\t%-16s\n", "akappa_e", "W_tot(akappa_e)/W0", "W_tot/W0");
    }

    double spectrum_tot = 0.0;
    for (int akappa_e = akappa_e_min; akappa_e <= akappa_e_max; akappa_e++)
    {
        
        if (options->print_level >= 1)
        {
            printf("--------------------------------------------------------------------------\n");
            printf("akappa_e = %3d\n", akappa_e);
            printf("--------------------------------------------------------------------------\n");
            printf("%16s\t%24s\t%16s\n\n", "energy_e [MeV]", "W(energy_e, akappa_e)/W0", "W_tot(akappa_e)/W0");
        }
        double spectrum_partial = 0.0;
        for (int ie = 0; ie < qgrid_e->n; ie++)
        {
            double e_e = MeV_to_RU(*(qgrid_e->x + ie));
            double weight_e = *(qgrid_e->w + ie);
    
            double transrate_partial = partial_transrate_mu_decay(znucl, n_muon, kappa_muon, e_e, akappa_e);

            spectrum_partial += transrate_partial * weight_e;

            if (options->print_level >= 1)
            {
                printf("%16.6f\t%24.12e\t%16.12e\n", e_e*me_MeV, transrate_partial/me_MeV, spectrum_partial/me_MeV);
            }
        }
        spectrum_tot += spectrum_partial;

        if (options->print_level >= 1)
        {
            printf("--------------------------------------------------------------------------\n");
            printf("%-16s\t%16s\t%-16s\n", "akappa_e", "W_tot(akappa_e)/W0", "W_tot/W0");
        }
        printf("%16d\t%16.12e\t%16.12e\n", akappa_e, spectrum_partial/me_MeV, spectrum_tot/me_MeV);
    }



    restore_stack_pos(pos_start);

    // print_fqgrid_stack();

    delete_qgrid(qgrid_e);
    delete_qgrid(qgrid_1);



    timer_stop("tot");
    timer_stats();

    return 1;
}
