#ifndef OPTIONS_H_INCLUDED
#define OPTIONS_H_INCLUDED


typedef struct
{
    double r1;
    double r2;
    
    int znucl;
    int inuclear_model;
    double rms;

    int kappa_muon;
    int n_muon;
    
    int akappa_e_min;
    int akappa_e_max;
    
    double energy_e_min;
    double energy_e_max;
    
    int nsub_r;
    int nqnode_r;

    int nsub_k;
    int nqnode_k;
    
    int nsub_e;
    int nqnode_e;    
    
    int print_level;

} options_t;

extern options_t* options;

options_t* new_options(void);

void delete_options(options_t* options);

void print_options(options_t* options);


enum
{
    PRINT_LEVEL_LOW,
    PRINT_LEVEL_HIGH
};


#endif