#ifndef SAMPLING_H
#define SAMPLING_H

#include"binspec.h"
#include"histogram-utils.h"
#include <THnSparse.h>

//Samples a function at the center of each filled bin in the sparse
//histogram, placing the results in an identically binned sparse
//histogram
THnSparseF* sample(double (*fn) (const double*),
                   THnSparseF* hist,
                   std::string histname="hist");

#endif
