#include "sampling.h"
//debug
#include <iostream>
//end debug

THnSparseF* sample(double (*fn)(const double*),
                   THnSparseF* hist,
                   std::string histname)
{
  BinSpecs binspecs = histogram2binspecs(hist);
  long ndims = binspecs.size();
  THnSparseF* result =
    binspecs2histogram(binspecs,histname.c_str());

  long npoints = hist->GetNbins();

  int raw_bin_indices[ndims];
  std::vector<int> bin_indices(ndims);

  for(int bin = 0; bin < npoints; ++bin) {
    hist->GetBinContent(bin,raw_bin_indices);
    //raw bin indices are not zero-indexed
    for(int j = 0; j < ndims; ++j)
      bin_indices[j] = raw_bin_indices[j];

    //Check indices to see if they are overflow/underflow:
    if(!indicesinbounds(hist,bin_indices)) {
      std::cout << "Warning: Index out of bounds, skipping point "
                << bin
                << std::endl;
      printoutofbounds(hist,bin_indices);
      continue;
    }

    //Find center:
    std::vector<double> center
      = bincenter(hist, bin_indices);

    //Get the value at the center of the bin:
    double x[ndims];
    for(int j = 0; j < ndims; ++j) {
      x[j] = center[j];
    }
    double fval = fn(x);

    //Store in the histogram result:
    result->Fill(x,fval);
  }

  return result;
}
