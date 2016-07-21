#include <sstream>
#include <iostream>

#include "mex.h"
#include "mwg_decode.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
    //redirect stdout
    std::stringstream outputStream;
    std::cout.rdbuf(outputStream.rdbuf()); 

    //Get raw input
    char inputCharString[9];
    mxGetString(prhs[0], inputCharString, 9);
    inputCharString[8] = '\0';
    std::string instString(inputCharString);

    int retval = mwg_decode(instString);

    double* output = NULL;
    plhs[0] = mxCreateDoubleMatrix(1,1,mxREAL);
    plhs[1] = mxCreateString(outputStream.str().c_str());
    output = mxGetPr(plhs[0]);
    *output = static_cast<double>(retval);
}
