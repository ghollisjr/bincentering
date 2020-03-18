/*
  A BinSpec object is a struct which states the minimum and maximum
  bin edges as well as the number of bins in that dimension.

  A BinSpecs object is a vector of BinSpec objects.
 */

#ifndef BINSPEC_H
#define BINSPEC_H

#include <vector>

struct BinSpec {
  double low;
  double high;
  long nbins;

  const double delta() {
    return (high-low)/nbins;
  }
};

typedef std::vector<BinSpec> BinSpecs;

///Functions for working with BinSpec objects:

//Get the center of the nth bin, 0 indexed
double bincenter(BinSpec binspec, int bin);
std::vector<double> bincenter (BinSpecs& binspecs,
                               std::vector<int>& bin);

//Get the bin edges of the nth bin, 0 indexed
std::vector<double> binedges(BinSpec binspec, int bin);

//Same for each dimension in BinSpecs object
std::vector<std::vector<double> >
binedges(BinSpecs binspecs,
         std::vector<int> bin);
#endif
