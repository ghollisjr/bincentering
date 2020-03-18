#include "parse.h"

//Input arguments:
std::string infilename;
std::string histname;
std::string outfilename;

//Utility function for reading a string
std::string read_string(std::istream& is)
{
  std::string line;
  getline(is,line);

  std::string result;

  //State machine has the following states:
  //
  //0: Dropping until first quote is found
  //1: Reading content until final quote is found
  //2: Found last quote, end processing
  int state = 0;
  for(int i = 0; i < line.size(); ++i) {
    //Drop initial non-quote characters:
    if(state == 0) {
      if(line[i] == '"') state = 1;
    }
    else if(state == 1) {
      if(line[i] == '"') state = 2;
      else result.push_back(line[i]);
    }
    else if(state == 2) {
      break;
    }
  }
  return result;
}

//Parsing input arguments:
void parseinput(std::istream& is)
{
  std::string line;
  std::stringstream ss;
  //Read input into memory
  while(getline(is,line)) {
    //Remove comments:
    for(int i = 0; i < line.size(); ++i) {
      if(line[i] == '#') {
        for(; i < line.size(); ++i) {
          line[i] = ' ';
        }
      }
    }
    ss << line << std::endl;
  }
  //Extract values:

  //Input file:
  infilename = read_string(ss);
  //histogram name:
  histname = read_string(ss);
  //Output file:
  outfilename = read_string(ss);

  //Beam energy:
  ss >> beam_energy;
  //helicity:
  ss >> helicity;
  //model option:
  ss >> model_opt;
  //Channel option:
  ss >> channel_opt;

  //debug
  std::cout << ss.str() << std::endl;
  //end debug
}
