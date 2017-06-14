/*
 * Author: Mark Gottscho
 * Email: mgottscho@ucla.edu
 */

#include <string>
#include <iostream>

#include "mwg_decode.h"

int main(int argc, const char *argv[])
{
    if (argc != 2) {
        std::cout << "Usage: riscvdecode <INST>" << std::endl;
        std::cout << "where <INST> is a 32-bit RV64G instruction specified in BIG-ENDIAN hexadecimal format, DEADBEEF -- do not include the 0x or 0h prefix." << std::endl;
        return 1;
    }

    //Get raw input
    std::string instString(argv[1]);
    return mwg_decode(instString);
}
