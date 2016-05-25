#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cerrno>
#include <vector>
#include <map>
#include <functional>
#include <algorithm>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <stdint.h>

#include "riscv-types.h"
#include "riscv-endian.h"
#include "riscv-format.h"
#include "riscv-meta.h"
#include "riscv-elf.h"
#include "riscv-elf-file.h"
#include "riscv-elf-format.h"
#include "riscv-imm.h"
#include "riscv-decode.h"

#include "mex.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
    /*if(nrhs != 1) {
            mexErrMsgIdAndTxt("MyToolbox:arrayProduct:nrhs",
                                  "One input required.");
    }
    if(nlhs != 1) {
            mexErrMsgIdAndTxt("MyToolbox:arrayProduct:nlhs",
                                  "One output required.");
    }*/

    /* make sure the first input argument is a string */
    /*if( !mxIsString(prhs[0]) || 
                  mxGetNumberOfElements(prhs[0]) != 1 ) {
            mexErrMsgIdAndTxt("MyToolbox:arrayProduct:notString",
                                  "Input must be a string.");
    }*/
   
    //redirect stdout
    std::stringstream outputStream;
    std::cout.rdbuf(outputStream.rdbuf()); 

    //Get raw input
    char inputCharString[9];
    mxGetString(prhs[0], inputCharString, 8);
    inputCharString[8] = '\0';
    std::string instString(inputCharString);
    std::cout << "Raw input: " << instString << std::endl;
    
    std::stringstream ss;
    uint32_t raw;
    ss << std::hex << instString;
    ss >> raw;

    std::cout.fill('0');
    std::cout << "Interpreted as: 0x" << std::hex << std::setw(8) << raw << std::dec << std::endl;
    std::cout.fill(' ');
    
    struct riscv_decode dec;
	memset(&dec, 0, sizeof(dec));
    dec.inst = raw;
	riscv_decode_opcode<false,true,true,true,true,true,true,true,false>(dec, dec.inst); //RV64G without compressed inst
	riscv_decode_type(dec, dec.inst);

    std::cout << "Disassembly: ";
    std::cout << riscv_instruction_name[dec.op] << std::endl;
    std::cout << "Result: ";
    if (dec.op == riscv_op_unknown) {
        std::cout << "ILLEGAL" << std::endl;
    } else {
        std::cout << "valid" << std::endl;
    }

    double* output = NULL;
    plhs[0] = mxCreateDoubleMatrix(1,1,mxREAL);
    plhs[1] = mxCreateString(outputStream.str().c_str());
    output = mxGetPr(plhs[0]);
    if (dec.op == riscv_op_unknown) {
        *output = 1;
    else
        *output = 0;
}
