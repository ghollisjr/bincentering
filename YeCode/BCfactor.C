#include "Math/AdaptiveIntegratorMultiDim.h"
#include <Math/WrappedParamFunction.h>
#include <maid.h>
//double f( double *x, double *par){
//   return par[0]*x[0]+par[1]*x[1]+par[2]*x[2]+par[3]*x[3]+par[4];
//}

double f( const double *x, const double *p){
   float W,Q2,costheta,phi,v,beam_energy;
   int   helicity,model_opt,channel_opt,resonance_opt; 
   W = x[0]; Q2 =x[1]; costheta = x[2]; phi = x[3];
   beam_energy = p[0]; helicity=p[1]; model_opt=p[2] ; channel_opt = p[3]; resonance_opt=p[4];
   v = maid_dsigma(beam_energy,W,Q2,costheta,phi,helicity,model_opt,channel_opt,resonance_opt); 
   return v;  
}

int NumericalIntegration()
{

    
   // Create the function and wrap it
   //TF2 f("Sin Function", "sin(x) + y", 0, TMath::Pi(), 0, 1);
   //  ROOT::Math::WrappedMultiTF1 wf1(f);
   ROOT::Math::WrappedParamFunction<> wf( &f, 4, 5);
   double p[5] = {2.039,0,5,2,0};
   wf.SetParameters(p);
   // Create the Integrator
   ROOT::Math::AdaptiveIntegratorMultiDim ig;
   // Set parameters of the integration
   ig.SetFunction(wf);
   ig.SetRelTolerance(0.001);
   double xmin[] = {1.1,0.4,-1,0};
   double xmax[] = {1.125,0.6,-0.8,40};
   cout << ig.Integral(xmin, xmax) << endl;
   return 0;
}

