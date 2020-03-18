#include <THnSparse.h>
#include <TFile.h>
#include <iostream>

int main(int argc, char** argv)
{
  const int nargs = 3;
  if(argc != nargs + 1) {
    std::cerr << "Usage: convert <infile> <histname> <outfile>"
              << std::endl << std::endl
              << "This program removes the momentum dimension from Ye's histogram"
              << std::endl;
    return 1;
  }
  TFile infile(argv[1],"READ");
  const char* histname = argv[2];
  THnSparseF* hist =
    (THnSparseF*) infile.Get(histname);

  TFile outfile(argv[3],"RECREATE");
  outfile.cd();

  //Project histogram into only the first four dimensions
  int ndim = 4;
  int dim[] = {0,1,2,3};

  THnSparseF* h = (THnSparseF*) hist->Projection(ndim,dim);

  //Write histogram to output
  h->Write();
  outfile.Close();
}
