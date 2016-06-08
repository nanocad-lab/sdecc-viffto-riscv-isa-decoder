#include "mwg_decode.h"

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
//#include <deque>

#include "riscv-types.h"
#include "riscv-endian.h"
#include "riscv-format.h"
#include "riscv-meta.h"
#include "riscv-elf.h"
#include "riscv-elf-file.h"
#include "riscv-elf-format.h"
#include "riscv-imm.h"
#include "riscv-decode.h"
//#include "riscv-disasm.h"

int mwg_decode(std::string instString) {
    int retval = 1;

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

    //Spit out legality first
    bool legal_op = false;
    std::cout << "Legality: ";
    if (dec.op == riscv_op_unknown) {
        std::cout << "ILLEGAL" << std::endl;
        retval = 1;
        legal_op = false;
    } else {
        std::cout << "valid" << std::endl;
        retval = 0;
        legal_op = true;
    }

    //Operation mneumonic
    std::cout << "Mneumonic: ";
    if (legal_op)
        std::cout << riscv_instruction_name[dec.op] << std::endl;
    else
        std::cout << "NA" << std::endl;

    //Floating pt op?
    bool float_op = (riscv_instruction_name[dec.op][0] == 'f');

    //Codec
    enum riscv_codec codec = riscv_instruction_codec[dec.op];
    bool legal_codec = true;
    std::cout << "Codec: ";
    switch (codec) {
        default:                        std::cout << "FAILURE" << std::endl;    legal_codec = false;    break; 
        case riscv_codec_unknown:       std::cout << "unknown" << std::endl;    legal_codec = false;    break; 
        case riscv_codec_none:          std::cout << "none" << std::endl;       legal_codec = false;    break;
        /*case riscv_codec_cb:            std::cout << "cb" << std::endl;         break;
        case riscv_codec_cb_sh5:        std::cout << "cb_sh5" << std::endl;     break;
        case riscv_codec_ci:            std::cout << "ci" << std::endl;         break;
        case riscv_codec_ci_sh5:        std::cout << "ci_sh5" << std::endl;     break;
        case riscv_codec_ci_16sp:       std::cout << "ci_16sp" << std::endl;    break;
        case riscv_codec_ci_lwsp:       std::cout << "ci_lwsp" << std::endl;    break;
        case riscv_codec_ci_ldsp:       std::cout << "ci_ldsp" << std::endl;    break;
        case riscv_codec_ci_li:         std::cout << "ci_li" << std::endl;      break;
        case riscv_codec_ci_lui:        std::cout << "ci_lui" << std::endl;     break;
        case riscv_codec_ci_nop:        std::cout << "ci_nop" << std::endl;     break;
        case riscv_codec_ciw_4spn:      std::cout << "ciw_4spn" << std::endl;   break;
        case riscv_codec_cj:            std::cout << "cj" << std::endl;         break;
        case riscv_codec_cl_lw:         std::cout << "cl_lw" << std::endl;      break;
        case riscv_codec_cl_ld:         std::cout << "cl_ld" << std::endl;      break;
        case riscv_codec_cr:            std::cout << "cr" << std::endl;         break;
        case riscv_codec_cr_mv:         std::cout << "cr_mv" << std::endl;      break;
        case riscv_codec_cr_jalr:       std::cout << "cr_jalr" << std::endl;    break;
        case riscv_codec_cr_jr:         std::cout << "cr_jr" << std::endl;      break;
        case riscv_codec_cs:            std::cout << "cs" << std::endl;         break;
        case riscv_codec_cs_sw:         std::cout << "cs_sw" << std::endl;      break;
        case riscv_codec_cs_sd:         std::cout << "cs_sd" << std::endl;      break;
        case riscv_codec_css_swsp:      std::cout << "css_swsp" << std::endl;   break;
        case riscv_codec_css_sdsp:      std::cout << "css_sdsp" << std::endl;   break;*/
        case riscv_codec_i:             std::cout << "i" << std::endl;          break;
        case riscv_codec_i_sh5:         std::cout << "i_sh5" << std::endl;      break;
        case riscv_codec_i_sh6:         std::cout << "i_sh6" << std::endl;      break;
        case riscv_codec_r:             std::cout << "r" << std::endl;          break;
        case riscv_codec_r_m:           std::cout << "r_m" << std::endl;        break;
        case riscv_codec_r_4:           std::cout << "r_4" << std::endl;        break;
        case riscv_codec_r_a:           std::cout << "r_a" << std::endl;        break;
        case riscv_codec_r_l:           std::cout << "r_l" << std::endl;        break;
        case riscv_codec_s:             std::cout << "s" << std::endl;          break;
        case riscv_codec_sb:            std::cout << "sb" << std::endl;         break;
        case riscv_codec_u:             std::cout << "u" << std::endl;          break;
        case riscv_codec_uj:            std::cout << "uj" << std::endl;         break;
    }

    //rd
    std::cout << "rd: ";
    if (!legal_codec || codec == riscv_codec_sb || codec == riscv_codec_s) {
        std::cout << "NA" << std::endl;
    } else {
        if (!float_op)
            std::cout << riscv_i_registers[dec.rd] << std::endl;
        else
            std::cout << riscv_f_registers[dec.rd] << std::endl;
    }
    
    //rs1
    std::cout << "rs1: ";
    if (!legal_codec || codec == riscv_codec_u || codec == riscv_codec_uj) {
        std::cout << "NA" << std::endl;
    } else {
        if (!float_op)
            std::cout << riscv_i_registers[dec.rs1] << std::endl;
        else
            std::cout << riscv_f_registers[dec.rs1] << std::endl;
    }
    
    //rs2
    std::cout << "rs2: ";
    if (!legal_codec || codec == riscv_codec_u || codec == riscv_codec_uj || codec == riscv_codec_i) {
        std::cout << "NA" << std::endl;
    } else {
        if (!float_op)
            std::cout << riscv_i_registers[dec.rs2] << std::endl;
        else
            std::cout << riscv_f_registers[dec.rs2] << std::endl;
    }
    
    //rs3
    std::cout << "rs3: ";
    if (codec == riscv_codec_r_4) {
        if (!float_op)
            std::cout << "NA" << std::endl;
        else
            std::cout << riscv_f_registers[dec.rs3] << std::endl;
    } else {
        std::cout << "NA" << std::endl;
    }
    
    //imm
    std::cout << "imm: ";
    if (!legal_codec || codec == riscv_codec_r) {
        std::cout << "NA" << std::endl;
    } else {
        std::cout.fill('0');
        std::cout << " 0x" << std::hex << std::setw(16) << dec.imm << std::endl;
    }
    
    //arg
    std::cout << "arg: ";
    if (codec == riscv_codec_r_m || codec == riscv_codec_r_l || codec == riscv_codec_r_a || codec == riscv_codec_r_m) {
        if (float_op) {
            std::cout.fill('0');
            std::cout << " 0x" << std::hex << std::setw(16) << dec.arg << std::endl;
        } else {
            std::cout << "NA" << std::endl;
        }
    } else {
        std::cout << "NA" << std::endl;
    }

    //Disassembly
    //std::deque<struct riscv_disasm> dec_hist;
    //std::cout << "disassembly: ";
    //riscv_disasm_instruction(dec,dec_hist,0,0,0,0,0,0);

    return retval;
}
