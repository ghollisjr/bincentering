# Modified Makefile from maid_xsection example project.  Original
# comments:
#
# This Makefile shows how to compile a program called "example" which
# consists of a single source code file, example.cc.  Simply change
# the variables PROGRAM and SOURCE to match what you want for your
# program.
#
# The key pieces to note are
#
# 1. gfortran must be used to link the code.  I've tried to find ways
#    to make g++ link to the Fortran libraries appropriately, but it
#    is more difficult than it's worth.
#
# 2. Keep the order of the gfortran flags, library location flags,
#    etc. as they are in the program compilation recipe.  I have no
#    idea why, but changing the order sometimes breaks the compilation
#    when I tested it.  This is the order I've found to work in
#    general.

# Need to modify these in the future since I will likely want more
# than a single source code file for the bincentering project,
# especially since I want to include the argument parser library.


# Compilation and linking settings:
FORTRAN_FLAGS = -O3 -m64 -fno-automatic -ffixed-line-length-none -fno-second-underscore
LIBRARYLOC_FLAGS= -L$(HOME)/slib/Linux64RHEL5 -L$(HOME)/lib/Linux64RHEL5 -L/group/clas/builds/PRODUCTION/slib/Linux64RHEL5 -L/u/home/gleb/lib/LinuxRHFC8 -L/apps/cernlib/x86_64_rhel7/2005/lib -L/usr/lib -L/usr/X11R6/lib64/ -L/usr/lib/mysql
LIBRARY_FLAGS = -lclasutil -lfputil -lrecutl -lbos -lfpack -lonline_dummy -lc_bos_io -llepto651 -ljetset74 -lpacklib -lmathlib -lkernlib -llepto651 -lfputil -lbos -lfpack -lstdc++ -lnsl -ldl -lgfortran `root-config --libs`
COMPILE_FLAGS = `root-config --cflags`

PROGS = bincentering convert test/parsetest test/compare
LIBOBJS = histogram-utils.o integrate.o binspec.o sampling.o dsigma.o parse.o

# Targets:
all: $(PROGS)

# Bin centering main program:
bincentering: bincentering.o $(LIBOBJS)
	gfortran $(FORTRAN_FLAGS) bincentering.o $(LIBOBJS) $(LIBRARYLOC_FLAGS) -lmaid_xsection $(LIBRARY_FLAGS) -o bincentering
bincentering.o: bincentering.cc
	g++ $(COMPILE_FLAGS) -c bincentering.cc -o bincentering.o

# Library functions:
histogram-utils.o: histogram-utils.cc
	g++ $(COMPILE_FLAGS) -c histogram-utils.cc -o histogram-utils.o
integrate.o: integrate.cc
	g++ $(COMPILE_FLAGS) -c integrate.cc -o integrate.o
binspec.o: binspec.cc
	g++ $(COMPILE_FLAGS) -c binspec.cc -o binspec.o
sampling.o: sampling.cc
	g++ $(COMPILE_FLAGS) -c sampling.cc -o sampling.o
dsigma.o: dsigma.cc
	g++ $(COMPILE_FLAGS) -c dsigma.cc -o dsigma.o
parse.o: parse.cc
	g++ $(COMPILE_FLAGS) -c parse.cc -o parse.o

# Convert Ye's 5-D histogram to a 4-D one that MAID can be compared
# against:
convert: convert.o
	g++ `root-config --libs` convert.o -o convert
convert.o: convert.cc
	g++ -c `root-config --cflags` convert.cc -o convert.o

# Testing
test/parsetest: test/parsetest.o $(LIBOBJS)
	gfortran $(FORTRAN_FLAGS) test/parsetest.o $(LIBOBJS) $(LIBRARYLOC_FLAGS) -lmaid_xsection $(LIBRARY_FLAGS) -o test/parsetest
test/parsetest.o: test/parsetest.cc
	g++ -c $(COMPILE_FLAGS) -I. test/parsetest.cc -o test/parsetest.o

test/compare: test/compare.o histogram-utils.o binspec.o
	g++ `root-config --libs` test/compare.o histogram-utils.o binspec.o -o test/compare
test/compare.o: test/compare.cc
	g++ -std=c++0x -c $(COMPILE_FLAGS) -I. test/compare.cc -o test/compare.o

clean:
	rm -f $(PROGS) *.o $(LIBOBJS)
