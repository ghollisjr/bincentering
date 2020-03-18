#include "dsigma.h"

float beam_energy;
int helicity, model_opt, channel_opt, resonance_opt = 0;

void init_dsigma(float be,
                 int hel,
                 int mopt,
                 int copt,
                 int ropt)
{
  beam_energy = be;
  helicity = hel;
  model_opt = mopt;
  channel_opt = copt;
  resonance_opt = ropt;
}

//Function which can be used with integration, assuming 
double dsigma(const double* x)
{
  const float W = (float) x[0];
  const float Q2 = (float) x[1];
  const float costheta = (float) x[2];
  const float phi = (float) x[3];
  return (double)
    maid_dsigma(beam_energy,
                W,Q2,costheta,phi,
                helicity,
                model_opt,channel_opt,resonance_opt);
}
