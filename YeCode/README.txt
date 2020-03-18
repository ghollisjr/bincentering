Ye's Email:

Hi Gary,

I put my data file and 4-dimensional function integration code
BCfactor.C in the attachment. If it is more convenient for you to use
yours, I will modify your code to my file format later. If you want to
use mine, in the data_phi986_test.root file, you can pick the
(THnSparseF) Qsq_W_Theta_Phi12_hist, which is 5 dimensional.

int dim_W = 0;
int dim_q2 = 1;
int dim_costheta = 2;
int dim_phi = 3;
int dim_ps =4;

And only the first four dimensions are needed to do the Bin Centring
Correction.  Thank you so much for your kind help and time.

Best regards,
Ye 
