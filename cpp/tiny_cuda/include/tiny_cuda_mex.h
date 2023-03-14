#include "mex.h"

const int INIT = 0;
const int DESTROY = 1;
const int FORWARD = 2;
const int BACKWARD = 3;
const int NUM_PARAMS = 4;

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]);

void forward(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]);

void backward(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]);

void init(int nlhs, mxArray *plhs[], int nrhs,
          const mxArray *prhs[]);

void destroy(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]);

void num_params(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]);
