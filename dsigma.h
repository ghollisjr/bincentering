#ifndef DSIGMA_H
#define DSIGMA_H

#include <maid.h>

extern float beam_energy;
extern int helicity, model_opt, channel_opt, resonance_opt;

void init_dsigma(float be,
                 int hel,
                 int mopt,
                 int copt,
                 int ropt=0);

//Function which can be used with integration, assuming 
double dsigma(const double* x);

#endif
