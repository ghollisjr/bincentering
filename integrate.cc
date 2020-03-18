#include "integrate.h"
#include "histogram-utils.h"
#include <iostream>

const char togglestr[] = "|/-\\|/-\\";
bool verbose_integration = false;
void verbose_messager(int point,int npoints)
{
  const char toggle1 = togglestr[point%8];
  const char toggle2 = togglestr[(point/8)%8];
  std::cout << "\r"
            << toggle2 << toggle1 << " "
            << "Integrating bin "
            << point
            << "/"
            << npoints
            << std::flush;
  //<< "\r";
  if(point >= npoints) {
    std::cout << std::endl
              << "Integration complete"
              << std::endl;
  }
}

//Function which integrates a function over a domain:
double integrate(double (*fn)(const double*),
                 std::vector<std::vector<double> > domain,
                 double tolerance)
{
  const long ndims = domain.size();
  double low[ndims];
  double high[ndims];
  for(int dim = 0; dim < ndims; ++dim) {
    low[dim] = domain[dim][0];
    high[dim] = domain[dim][1];
  }
  ROOT::Math::Functor wf(fn,ndims);
  ROOT::Math::IntegratorMultiDim ig(ROOT::Math::IntegrationMultiDim::kADAPTIVE);
  ig.SetFunction(wf);
  ig.SetRelTolerance(tolerance);
  return ig.Integral(low,high);
}

//Uses the histogram filled bins as separate domains and puts the
//integration results in the counts of a histogram result sharing
//identical binning
THnSparseF* integrate(double (*fn)(const double*),
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
    if(verbose_integration) {
      verbose_messager(bin,npoints);
    }

    hist->GetBinContent(bin,raw_bin_indices);
    //Place indices in a vector
    for(int j = 0; j < ndims; ++j)
      bin_indices[j] = raw_bin_indices[j];

    //Check indices to see if they are overflow/underflow:
    if(!indicesinbounds(hist,bin_indices)) {
      std::cout << "\nWarning: Index out of bounds, skipping point "
                << bin
                << std::endl;
      printoutofbounds(hist,bin_indices);
      continue;
    }

    //Find domain:
    std::vector<std::vector<double> > domain
      = binedges(hist, bin_indices);

    //Calculate the integral:
    const double integral = integrate(fn,domain);

    //Find center of the bin:
    std::vector<double> raw_center
      = bincenter(hist, bin_indices);
    double center[ndims];
    for(int j = 0; j < ndims; ++j)
      center[j] = raw_center[j];

    //Store in the histogram result:
    result->Fill(center,integral);
  }
  if(verbose_integration) {
    verbose_messager(npoints,npoints);
  }

  return result;
}

//Averaging functions:

double average(double (*fn)(const double*),
               std::vector<std::vector<double> > domain)
{
  double domain_volume = 1;
  int ndims = domain.size();
  for(int dim = 0; dim < ndims; ++dim) {
    const double domain_size = domain[dim][1] - domain[dim][0];
    domain_volume *= domain_size;
  }
  return integrate(fn,domain)/domain_volume;
}

THnSparseF* average(double (*fn)(const double*),
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
    if(verbose_integration) {
      verbose_messager(bin,npoints);
    }
    hist->GetBinContent(bin,raw_bin_indices);
    //place in a vector
    for(int j = 0; j < ndims; ++j)
      bin_indices[j] = raw_bin_indices[j];

    //Check indices to see if they are overflow/underflow:
    if(!indicesinbounds(hist,bin_indices)) {
      std::cout << "\nWarning: Index out of bounds, skipping point "
                << bin
                << std::endl;
      printoutofbounds(hist,bin_indices);
      continue;
    }

    //Find domain:
    std::vector<std::vector<double> > domain
      = binedges(hist, bin_indices);

    //Calculate the integral:
    const double mean = average(fn,domain);

    //Find center of the bin:
    std::vector<double> raw_center
      = bincenter(hist, bin_indices);
    double center[ndims];
    for(int j = 0; j < ndims; ++j)
      center[j] = raw_center[j];

    //Store in the histogram result:
    result->Fill(center,mean);
  }
  if(verbose_integration) {
    verbose_messager(npoints,npoints);
  }

  return result;
}
