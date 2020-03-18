/*
  histogram-utils provides functions which can read binning
  information about histograms either located in TFiles or already in
  memory and returns a binspec object.
 */

#ifndef HISTOGRAM_UTILS_H
#define HISTOGRAM_UTILS_H

#include <iostream>
#include <string>
#include <vector>
#include <TFile.h>
#include <THnSparse.h>
#include <TAxis.h>
#include "binspec.h"

bool indexinbounds(TAxis* axis, int index);
bool indicesinbounds(THnSparseF* h, std::vector<int> indices);

void printoutofbounds(THnSparseF* h, std::vector<int> indices);

//Bin functions since ROOT is doing spooky things with the bin indices
double bincenter(TAxis* axis, int bin);
std::vector<double> binedges(TAxis* axis, int bin);

std::vector<double> bincenter(THnSparseF* hist,
                              std::vector<int> bin);
std::vector<std::vector<double> >
binedges(THnSparseF* hist,
         std::vector<int> bin);

//returning binspecs from histograms
BinSpecs histogram2binspecs(THnSparseF* h);

//creating histogram from binspecs
THnSparseF* binspecs2histogram(const BinSpecs& binspecs,
                              std::string histname);

//reading histograms from files
THnSparseF* read_histogram(TFile* f, std::string name);
#endif
