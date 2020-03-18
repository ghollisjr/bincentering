#include <iostream>
#include <sstream>
#include <TFile.h>
#include <THnSparse.h>

#include "binspec.h"
#include "histogram-utils.h"
#include "integrate.h"
#include "sampling.h"
#include "dsigma.h"
#include "parse.h"

int main(int argc, char** argv)
{
  //Enable verbose integration:
  verbose_integration = true;

  //Argument parsing:
  const int nargs = 0;
  if(argc != (nargs+1)) {
    std::cout << "Feed arguents through stdin, see bincentering.inp for formatting."
              << std::endl << std::endl
              << "bincentering produces N result \"histograms\" in"
              << std::endl
              << "the output file:"
              << std::endl << std::endl
              << "1. The MAID cross section evaluated at the center"
              << std::endl
              << "of each bin from the input histogram," << std::endl
              << "2. The integrated MAID cross section in each bin" << std::endl
              << "3. The average MAID cross section in each bin" << std::endl
              << "4. The ratio center/average (hist 1 / hist 3)"
              << std::endl;
    return 1;
  }

  //Initialization:
  parseinput(std::cin);

  TFile infile(infilename.c_str(),"READ");
  THnSparseF* hist = (THnSparseF*) infile.Get(histname.c_str());

  TFile outfile(outfilename.c_str(),"RECREATE");
  outfile.cd();

  //Sampling doesn't throw errors

  THnSparseF* center = sample(dsigma,
                              hist,
                              "center");
  center->Write();

  THnSparseF* integral = integrate(dsigma,
                                   hist,
                                   "integral");
  integral->Write();

  THnSparseF* mean = average(dsigma,
                             hist,
                             "average");
  mean->Write();

  THnSparseF* center_over_mean
    = (THnSparseF*) hist->Clone("center_over_average");
  center_over_mean->Divide(center,mean);
  center_over_mean->Write();

  outfile.Close();
  return 0;
}
