#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <sstream>
#include <THnSparse.h>
#include <TFile.h>

#include "histogram-utils.h"
// #include "integrate.h"
#include "binspec.h"
// #include "sampling.h"
// #include "dsigma.h"
// #include "parse.h"

/*
  Histograms as implemented by ROOT are essentially a map from an
  N-dimensional point to a real number.  The difference between maps
  is the difference between each map on a point by point basis, but
  the catch with sparse histograms is that the maps only contain
  partial domains, so there is not an obvious and efficient way to
  iterate over an appropriate domain.

  Although technically all that's needed is the list of points from
  the domain at which the histograms differ, this struct contains
  additional information so that the results can be efficiently used
  later.

  type can be either 1 for being present in hist1 but not hist2, and 2
  for being present in hist2 but not hist1.
 */

TAxis** getaxes(THnSparseF* h)
{
  int ndims = h->GetNdimensions();
  TAxis** result = new TAxis*[ndims];

  for(int i = 0; i < ndims; ++i) {
    result[i] = h->GetAxis(i);
  }
  return result;
}

void lookuppoint(THnSparseF* h,
                 int* indices,
                 double* point)
{
  int ndims = h->GetNdimensions();
  TAxis **axes = getaxes(h);
  for(int i = 0; i < ndims; ++i) {
    point[i] = axes[i]->GetBinCenter(indices[i]);
  }
  delete[] axes;
  return;
}

struct Diff
{
  std::vector<double> point;
  int index;
  int type;
};

typedef std::list<Diff> Diffs;

/*Function for calculating the difference between two histograms
  (hist1 - hist2)+(hist2 - hist1).  Difference is a Diffs object.
  This function does not compare the values, only the difference in
  domain spanned.
*/
Diffs* hist_domain_xor(THnSparseF* hist1,
                       THnSparseF* hist2)
{
  int ndims = hist1->GetNdimensions();
  // setup diff variable
  Diff diff;
  diff.point.resize(ndims);
  double x[ndims];
  Diffs *result = new Diffs;

  //Check bins from hist1's perspective
  int indices[ndims];
  std::vector<int> indicesv(ndims);
  int index1, index2;
  //hist1 - hist2
  int npoints = hist1->GetNbins();
  for(index1 = 0; index1 < npoints; ++index1) {
    hist1->GetBinContent(index1,indices);
    for(int i = 0; i < ndims; ++i)
      indicesv[i] = indices[i];
    lookuppoint(hist1,indices,x);
    index2 = hist2->GetBin(x,false);
    if(index2 < 0) {
      for(int i = 0; i < ndims; ++i)
        diff.point[i] = x[i];
      diff.index = index1;
      diff.type = 1;
      result->push_back(diff);
    }
  }
  //hist2 - hist1
  npoints = hist2->GetNbins();
  for(index2 = 0; index2 < npoints; ++index2) {
    hist2->GetBinContent(index2,indices);
    for(int i = 0; i < ndims; ++i)
      indicesv[i] = indices[i];
    lookuppoint(hist2,indices,x);
    index1 = hist1->GetBin(x,false);
    if(index1 < 0) {
      for(int i = 0; i < ndims; ++i)
        diff.point[i] = x[i];
      diff.index = index1;
      diff.type = 2;
      result->push_back(diff);
    }
  }
  return result;
}

void printhistlimits(THnSparseF* hist,
                     std::string histname)
{
  int ndims = hist->GetNdimensions();
  int nbins = hist->GetNbins();
  TAxis** axes = getaxes(hist);
  
  std::cout << histname << ":" << std::endl
            << "ndims = " << ndims
            << "," << std::endl
            << "nbins = " << nbins
            << "," << std::endl;
  //Print axis limits:
  std::cout << "Axis limits:" << std::endl
            << "[";
  for(int i = 0; i < ndims; ++i) {
    if(i != 0) std::cout << " ";
    std::cout << "[low="
              << axes[i]->GetBinLowEdge(1)
              << ","
              << "high="
              << axes[i]->GetBinUpEdge(axes[i]->GetNbins())
              << ","
              << "nbins="
              << axes[i]->GetNbins()
              << "]";
    if(i!=(ndims-1))
      std::cout << "," << std::endl;
  }
  std::cout << "]" << std::endl;
}

void printdiffhists(Diffs* diffs)
{
  for(auto i: (*diffs)) {
    const int ndims = i.point.size();
    //Print point
    std::cout << "{\n [";
    for(int j = 0; j < ndims; ++j) {
      std::cout << i.point[j];
      if(j != (ndims-1))
        std::cout << ",";
    }
    std::cout << "],\n";
    //Print index
    std::cout << " " << i.index << ",\n";
    //Print type:
    std::cout << " " << i.type << "}\n";
  }
  return;
}

int main(int argc, char** argv)
{
  const int nargs = 4;
  if(argc != (nargs+1)) {
    std::cerr << "Usage: compare <path1> <name1> <path2> <name2>"
              << std::endl;
    return 1;
  }
  
  TFile infile1(argv[1],"READ");
  std::string name1 = argv[2];
  THnSparseF* hist1 = (THnSparseF*) infile1.Get(name1.c_str());
  
  TFile infile2(argv[3],"READ");
  std::string name2 = argv[4];
  THnSparseF* hist2 = (THnSparseF*) infile2.Get(name2.c_str());

  Diffs* diff = 
    hist_domain_xor(hist1, hist2);

  printhistlimits(hist1,"Hist1");
  printhistlimits(hist2,"Hist2");

  printdiffhists(diff);

  delete diff;
  
  return 0;
}
