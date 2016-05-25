//#include "MyDecoder.hh"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <stdint.h>

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "Usage: mipsdecode <INST>" << std::endl;
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
    
    //RiscvISA::ExtMachInst inst = static_cast<RiscvISA::ExtMachInst>(raw);

    //RiscvISA::Decoder decoder;
    std::cout << "Disassembly: ";
    //bool ret = decoder.decodeInst(inst);
    std::cout << "Result: ";
    if (ret) {
        std::cout << "ILLEGAL" << std::endl;
        return ret;
    } else {
        std::cout << "valid" << std::endl;
        return ret;
    }
}
