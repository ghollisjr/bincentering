----------------------------------------------------------------------
1. BASIC DESCRIPTION
----------------------------------------------------------------------
bincentering uses the MAID cross section tables to generate a bin by
bin correction factor for a histogram.  Because each different
histogram binning requires a different integration calculation, this
program uses an example histogram contained inside of a ROOT file to
find the binning, and then calculates the following "histograms":

1. center
2. integral
3. average
4. center_over_average

The "center" histogram is just the value of the MAID model at the
center of each bin.

The "integral" histogram is the integral of the MAID model over each
bin.

The "average" histogram is the integral divided by bin volume for each
bin.

The "center_over_average" histogram is the ratio between the MAID
model at the center of a bin and the average model value over that
same bin.  Therefore, center_over_average is a bin by bin correction
factor for a yield histogram, assuming that the MAID model provides a
good approximation to the curvature of the data.

----------------------------------------------------------------------
2. USING BINCENTERING
----------------------------------------------------------------------
bincentering.inp gives an example input file for bincentering.  Input
to bincentering needs to be given through standard input, so if you
store your configuration in a file, use bincentering like this:

./bincentering < bincentering.inp

to redirect the file to standard input.

The arguments in order are:

General control arguments:
1. The input file path,
2. The name of the histogram inside the input ROOT file,
3. The output file path,

MAID model arguments:
4. The beam energy of the experimental data,
5. The electron helicity,
6. The MAID model to use,
7. The reaction channel to investigate.

These are all commented in the input file as well.
