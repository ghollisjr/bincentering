#include "histogram-utils.h"

const char axisnameW[] = "W";
const char axisnameQ2[] = "Q^2";
const char axisnameCosTheta[] = "cos(theta*)";
const char axisnamePhi[] = "phi*";

const char* axisnames[4] = {axisnameW,
                            axisnameQ2,
                            axisnameCosTheta,
                            axisnamePhi};

//Bin functions since ROOT is doing spooky things with the bin indices
double bincenter(TAxis* axis, int bin)
{
  return axis->GetBinCenter(bin);
}

std::vector<double>
bincenter(THnSparseF* hist,
          std::vector<int> bin)
{
  long ndims = hist->GetNdimensions();
  std::vector<double> result(ndims);
  for(int dim = 0; dim < ndims; ++dim) {
    TAxis* axis = hist->GetAxis(dim);
    result[dim] = bincenter(axis,bin[dim]);
  }
  return result;
}

std::vector<double> binedges(TAxis* axis, int bin)
{
  std::vector<double> result(2);
  result[0] = axis->GetBinLowEdge(bin);
  result[1] = axis->GetBinUpEdge(bin);
  return result;
}

std::vector<std::vector<double> >
binedges(THnSparseF* hist,
         std::vector<int> bin)
{
  long ndims = hist->GetNdimensions();
  std::vector<std::vector<double> > result(ndims);
  for(int dim = 0; dim < ndims; ++dim) {
    TAxis* axis = hist->GetAxis(dim);
    result[dim] = binedges(axis,bin[dim]);
  }
  return result;
}

bool indexinbounds(TAxis* axis, int index)
{
  const double center = axis->GetBinCenter(index);
  return (axis->GetXmin() <= center) && (center <= axis->GetXmax());
}

bool indicesinbounds(THnSparseF* h, std::vector<int> indices)
{
  int ndims = h->GetNdimensions();
  for(int i = 0; i < ndims; ++i) {
    TAxis* axis = h->GetAxis(i);
    if(! indexinbounds(axis,indices[i])) return false;
  }
  return true;
}

void printoutofbounds(THnSparseF* h, std::vector<int> indices)
{
  int ndims = h->GetNdimensions();
  for(int i = 0; i < ndims; ++i) {
    TAxis* axis = h->GetAxis(i);
    if(! indexinbounds(axis,indices[i])) {
      std::cout << axisnames[i]
                << " index "
                << indices[i]
                << " out of bounds: "
                << "min=" << axis->GetXmin() << ", "
                << "point=" << axis->GetBinCenter(indices[i]) << ", "
                << "max=" << axis->GetXmax()
                << std::endl;
    }
  }
}

//returning binspecs from histograms
BinSpecs histogram2binspecs(THnSparseF* h)
{
  const int ndims = h->GetNdimensions();
  BinSpecs result(ndims);
  //Loop over axes returning binspec from each dimension:
  for(int dim = 0; dim < ndims; ++dim) {
    TAxis* axis = h->GetAxis(dim);
    result[dim].nbins = axis->GetNbins();
    result[dim].low = axis->GetBinLowEdge(1);
    result[dim].high = axis->GetBinUpEdge(result[dim].nbins);
  }
  return result;
}

//creating histogram from binspecs
THnSparseF* binspecs2histogram(const BinSpecs& binspecs,
                               std::string histname)
{
  long ndims = binspecs.size();
  double lows[ndims];
  double highs[ndims];
  int nbins[ndims];
  for(int dim = 0; dim < ndims; ++dim) {
    lows[dim] = binspecs[dim].low;
    highs[dim] = binspecs[dim].high;
    nbins[dim] = binspecs[dim].nbins;
  }
  return new THnSparseF(histname.c_str(),
                        histname.c_str(),
                        (int) ndims,
                        (int*) nbins,
                        (double*) lows,
                        (double*) highs);
}

//reading histograms from files
THnSparseF* read_histogram(TFile* f, std::string name)
{
  THnSparseF* result = (THnSparseF*) f->Get(name.c_str());
  return result;
}
