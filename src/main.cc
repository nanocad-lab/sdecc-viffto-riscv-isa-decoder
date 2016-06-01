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

int main(int argc, const char *argv[])
{
    if (argc != 2) {
        std::cout << "Usage: riscvdecode <INST>" << std::endl;
        std::cout << "where <INST> is a 32-bit RV64G instruction specified in BIG-ENDIAN hexadecimal format, DEADBEEF -- do not include the 0x or 0h prefix." << std::endl;
        return 1;
    }

    //Get raw input
    std::string instString(argv[1]);
    std::cout << "Raw input: " << instString << std::endl;
    
    std::stringstream ss;
    uint32_t raw;
    ss << std::hex << instString;
    ss >> raw;

    std::cout.fill('0');
    std::cout << "Interpreted as: 0x" << std::hex << std::setw(8) << raw << std::dec << std::endl;
    std::cout.fill(' ');

    struct riscv_disasm dec;
	memset(&dec, 0, sizeof(dec));
    dec.inst = raw;
	riscv_decode_opcode<riscv_disasm,false,true,true,true,true,true,true,true,false>(dec, dec.inst); //RV64G without compressed inst
	riscv_decode_type(dec, dec.inst);

    std::cout << "Disassembly: ";
    std::cout << riscv_instruction_name[dec.op] << std::endl;
    std::cout << "Result: ";
    if (dec.op == riscv_op_unknown) {
        std::cout << "ILLEGAL" << std::endl;
        return 1;
    } else {
        std::cout << "valid" << std::endl;
        return 0;
    }
}
