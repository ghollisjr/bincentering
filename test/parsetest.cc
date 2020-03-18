#include <iostream>
#include <sstream>
#include <THnSparse.h>
#include <TFile.h>

#include "histogram-utils.h"
#include "integrate.h"
#include "binspec.h"
#include "sampling.h"
#include "dsigma.h"
#include "parse.h"

using namespace std;

int main()
{
  parseinput(cin);

  cout << infilename << endl;
  cout << histname << endl;
  cout << outfilename << endl;

  cout << beam_energy << endl;
  cout << helicity << endl;
  cout << model_opt << endl;
  cout << channel_opt << endl;
  
  return 0;
}
