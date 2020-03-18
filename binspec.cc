#include "binspec.h"

//bin center functions
double bincenter(BinSpec binspec, int bin)
{
  const double delta = binspec.delta();
  return binspec.low + delta/2 + delta*bin;
}

std::vector<double> bincenter(BinSpecs& binspecs,
                              std::vector<int>& bin)
{
  const long ndims = binspecs.size();
  std::vector<double> result(ndims);
  for(int i = 0; i < ndims; ++i) {
    result[i] = bincenter(binspecs[i],bin[i]);
  }
  return result;
}

//bin edge functions
std::vector<double> binedges(BinSpec binspec, int bin)
{
  const double delta = binspec.delta();
  std::vector<double> result(2);
  result[0] = binspec.low + bin*delta;
  result[1] = binspec.low + (bin+1)*delta;
  return result;
}

std::vector<std::vector<double> >
binedges(BinSpecs binspecs,
         std::vector<int> bin)
{
  long ndims = binspecs.size();
  std::vector<std::vector<double> > result(ndims);
  for(int dim = 0; dim < ndims; ++dim) {
    result[dim] = binedges(binspecs[dim],bin[dim]);
  }
  return result;
}
