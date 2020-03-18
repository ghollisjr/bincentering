#ifndef PARSE_H
#define PARSE_H

#include <string>
#include <iostream>
#include <sstream>
#include "dsigma.h"

//Input arguments:
extern std::string infilename;
extern std::string histname;
extern std::string outfilename;

//Utility function for reading a string
std::string read_string(std::istream& is);

//Parsing input arguments:
void parseinput(std::istream& is);

#endif
