#ifndef INTEGRATE_H
#define INTEGRATE_H

#include <iostream>

#include"binspec.h"

#include <THnSparse.h>

#include<Math/Integrator.h>
#include<Math/IntegratorMultiDim.h>
#include<Math/AllIntegrationTypes.h>
#include<Math/GaussIntegrator.h>
#include<Math/Functor.h>

//Verbosity:

extern bool verbose_integration;
void verbose_messager(int point,int npoints);

//Function for integrating a multidimensional function using ROOT.
//
//
//domain should be a vector containing a 2-element double vectors of
//the form {low_domain_edge, high_domain_edge} for each dimension.
//
//The domain argument accepts the format returned by the binedges
//function directly.
double integrate(double (*fn)(const double*),
                 std::vector<std::vector<double> > domain,
                 double tolerance = 0.0001);

//Function which integrates over each filled bin in a THnSparseF
//object, returning another THnSparseF object where the count is set
//to the value of the integral of the function over each bin.
THnSparseF* integrate(double (*fn)(const double*),
                      THnSparseF* hist,
                      std::string histname="hist");

//Functions for calculating average values using the same mechanisms
//as the above two functions
double average(double (*fn)(const double*),
               std::vector<std::vector<double> > domain);

THnSparseF* average(double (*fn)(const double*),
                    THnSparseF* hist,
                    std::string histname="hist");

#endif
