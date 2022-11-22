#include "mex.h"

const int INIT = 0;
const int DESTROY = 1;
const int SETJOINTS = 2;
const int GETJOINTS = 3;
const int GETJACOB = 4;
const int GETBODYTRANS = 5;
const int GETJOINTTRANS = 6;
const int GETOPPOS = 7;
const int GETJOINTTREE = 8;
const int REVOLUTE = 9;
const int PRISMATIC = 10;
const int FIXED = 11;

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]);

void setJoints(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]);

void getJoints(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]);

void getOperationalPosition(int nlhs, mxArray *plhs[], int nrhs,
                            const mxArray *prhs[]);

void getJacobian(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]);

void getBodyTransform(int nlhs, mxArray *plhs[], int nrhs,
                      const mxArray *prhs[]);

void getJointTransform(int nlhs, mxArray *plhs[], int nrhs,
                       const mxArray *prhs[]);

void getJointTree(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]);