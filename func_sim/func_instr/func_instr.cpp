/*
 * func_instr.cpp - instruction parser for mips
 * @author Pavel Kryukov pavel.kryukov@phystech.edu
 * Copyright 2015 MIPT-MIPS
 */

#include <cstdlib>
#include <cstdio>

#include <iostream>
#include <sstream>
#include <iomanip>

#include "func_instr.h"

const FuncInstr::ISAEntry FuncInstr::isaTable[] =
{
    { "###",    0xFF,  FORMAT_UNKNOWN, OUT_R_ARITHM,  0, &FuncInstr::execute_unknown},

    // **************** R INSTRUCTIONS ****************
    // Constant shifts
    // name    funct   format    operation     memsize           pointer
    { "sll",    0x0,   FORMAT_R, OUT_R_SHAMT,      0, &FuncInstr::execute_sll},
    { "srl",    0x2,   FORMAT_R, OUT_R_SHAMT,      0, &FuncInstr::execute_srl},
    { "sra",    0x3,   FORMAT_R, OUT_R_SHAMT,      0, &FuncInstr::execute_sra},

    // Variable shifts
    // name    funct   format    operation     memsize           pointer
    { "sllv",   0x4,   FORMAT_R, OUT_R_ARITHM,     0, &FuncInstr::execute_sllv},
    { "srlv",   0x6,   FORMAT_R, OUT_R_ARITHM,     0, &FuncInstr::execute_srlv},
    { "srav",   0x7,   FORMAT_R, OUT_R_ARITHM,     0, &FuncInstr::execute_srav},
    
    // Indirect branches
    // name    funct    format    operation     memsize           pointer
    { "jr",     0x8,   FORMAT_R, OUT_R_JUMP,       0, &FuncInstr::execute_jr},
    { "jalr",   0x9,   FORMAT_R, OUT_R_JUMP_LINK,  0, &FuncInstr::execute_jalr},

    // System calls
    // name    funct    format    operation     memsize           pointer
    { "break",  0xD,   FORMAT_R, OUT_R_SPECIAL,    0, &FuncInstr::execute_break},
    { "syscall",0xC,   FORMAT_R, OUT_R_SPECIAL,    0, &FuncInstr::execute_syscall},
    
    // HI/LO manipulations
    // name    funct   format    operation     memsize           pointer
    { "mfhi",   0x10,  FORMAT_R, OUT_R_ARITHM,     0, &FuncInstr::execute_mfhi},
    { "mthi",   0x11,  FORMAT_R, OUT_R_ARITHM,     0, &FuncInstr::execute_mthi},
    { "mflo",   0x12,  FORMAT_R, OUT_R_ARITHM,     0, &FuncInstr::execute_mflo},
    { "mtlo",   0x13,  FORMAT_R, OUT_R_ARITHM,     0, &FuncInstr::execute_mtlo},
    
    // Multiplication/Division
    // name    funct    format    operation     memsize           pointer
    { "mult",   0x18,  FORMAT_R, OUT_R_ARITHM,     0, &FuncInstr::execute_mult},
    { "multu",  0x19,  FORMAT_R, OUT_R_ARITHM,     0, &FuncInstr::execute_multu},
    { "div",    0x1A,  FORMAT_R, OUT_R_ARITHM,     0, &FuncInstr::execute_div},
    { "divu",   0x1B,  FORMAT_R, OUT_R_ARITHM,     0, &FuncInstr::execute_divu},
    
    // Addition/Subtraction
    // name    funct    format    operation     memsize           pointer
    { "add",    0x20,  FORMAT_R, OUT_R_ARITHM,     0, &FuncInstr::execute_add},
    { "addu",   0x21,  FORMAT_R, OUT_R_ARITHM,     0, &FuncInstr::execute_addu},
    { "sub",    0x22,  FORMAT_R, OUT_R_ARITHM,     0, &FuncInstr::execute_sub},
    { "subu",   0x23,  FORMAT_R, OUT_R_ARITHM,     0, &FuncInstr::execute_subu},
    
    // Logical operations
    // name    funct    format    operation     memsize           pointer
    { "and",    0x24,  FORMAT_R, OUT_R_ARITHM,     0, &FuncInstr::execute_and},
    { "or",     0x25,  FORMAT_R, OUT_R_ARITHM,     0, &FuncInstr::execute_or},
    { "xor",    0x26,  FORMAT_R, OUT_R_ARITHM,     0, &FuncInstr::execute_xor},
    { "nor",    0x27,  FORMAT_R, OUT_R_ARITHM,     0, &FuncInstr::execute_nor},
    { "slt",    0x2A,  FORMAT_R, OUT_R_ARITHM,     0, &FuncInstr::execute_slt},
    { "sltu",   0x2B,  FORMAT_R, OUT_R_ARITHM,     0, &FuncInstr::execute_sltu},

    // ********************* I and J INSTRUCTIONS *************************
    // Branches
    // name    opcode    format    operation     memsize           pointer
    { "j",      0x2,   FORMAT_J, OUT_J_JUMP,       0, &FuncInstr::execute_j},
    { "jal",    0x3,   FORMAT_J, OUT_J_JUMP_LINK,  0, &FuncInstr::execute_jal},
    { "beq",    0x4,   FORMAT_I, OUT_I_BRANCH,     0, &FuncInstr::execute_beq},
    { "bne",    0x5,   FORMAT_I, OUT_I_BRANCH,     0, &FuncInstr::execute_bne},
    { "blez",   0x6,   FORMAT_I, OUT_I_BRANCH,     0, &FuncInstr::execute_blez},
    { "bgtz",   0x7,   FORMAT_I, OUT_I_BRANCH,     0, &FuncInstr::execute_bgtz},    
        
    // Addition/Subtraction
    // name    opcode    format    operation     memsize           pointer
    { "addi",   0x8,   FORMAT_I, OUT_I_ARITHM,     0, &FuncInstr::execute_addi},
    { "addiu",  0x9,   FORMAT_I, OUT_I_ARITHM,     0, &FuncInstr::execute_addiu},

    // Logical operations
    // name    opcode    format    operation     memsize           pointer
    { "slti",   0xA,   FORMAT_I, OUT_I_ARITHM,     0, &FuncInstr::execute_slti},
    { "sltiu",  0xB,   FORMAT_I, OUT_I_ARITHM,     0, &FuncInstr::execute_sltiu},
    { "andi",   0xC,   FORMAT_I, OUT_I_ARITHM,     0, &FuncInstr::execute_andi},
    { "ori",    0xD,   FORMAT_I, OUT_I_ARITHM,     0, &FuncInstr::execute_ori},
    { "xori",   0xE,   FORMAT_I, OUT_I_ARITHM,     0, &FuncInstr::execute_xori}, 
    { "lui",    0xF,   FORMAT_I, OUT_I_CONST,      0, &FuncInstr::execute_lui},
    
    // Coprocessor operations 0x10 - 0x13

    // Likely branches (MIPS II)
    // name    opcode    format    operation     memsize           pointer
    { "beql",   0x14,  FORMAT_I, OUT_I_BRANCH,     0, &FuncInstr::execute_beq},
    { "bnel",   0x15,  FORMAT_I, OUT_I_BRANCH,     0, &FuncInstr::execute_bne},
    { "blezl",  0x16,  FORMAT_I, OUT_I_BRANCH,     0, &FuncInstr::execute_blez},
    { "bgtzl",  0x17,  FORMAT_I, OUT_I_BRANCH,     0, &FuncInstr::execute_bgtz},    

    // System calls
    // name    opcode    format    operation     memsize           pointer
    { "trap",   0x1A,  FORMAT_J, OUT_J_SPECIAL,    0, &FuncInstr::execute_trap},

    // Loads
    // name    opcode    format    operation     memsize           pointer
    { "lb",     0x20,  FORMAT_I, OUT_I_LOAD,       1, &FuncInstr::calculate_load_addr},
    { "lh",     0x21,  FORMAT_I, OUT_I_LOAD,       2, &FuncInstr::calculate_load_addr},
 // { "lwl",    0x22,  FORMAT_I, OUT_I_LOAD,       4, &FuncInstr::calculate_load_addr},
    { "lw",     0x23,  FORMAT_I, OUT_I_LOAD,       4, &FuncInstr::calculate_load_addr},
    { "lbu",    0x24,  FORMAT_I, OUT_I_LOADU,      1, &FuncInstr::calculate_load_addr},
    { "lhu",    0x25,  FORMAT_I, OUT_I_LOADU,      2, &FuncInstr::calculate_load_addr},
 // { "lwr",    0x26,  FORMAT_I, OUT_I_LOAD,       4, &FuncInstr::calculate_load_addr},
    { "lwu",    0x27,  FORMAT_I, OUT_I_LOADU,      4, &FuncInstr::calculate_load_addr},

    // Stores
    // name    opcode    format    operation     memsize           pointer
    { "sb",     0x28,  FORMAT_I, OUT_I_STORE,      1, &FuncInstr::calculate_store_addr},
    { "sh",     0x29,  FORMAT_I, OUT_I_STORE,      2, &FuncInstr::calculate_store_addr},
 // { "swl",    0x2A,  FORMAT_I, OUT_I_STORE,      4, &FuncInstr::calculate_store_addr},
    { "sw",     0x2B,  FORMAT_I, OUT_I_STORE,      4, &FuncInstr::calculate_store_addr},
 // { "swr",    0x2E,  FORMAT_I, OUT_I_STORE,      4, &FuncInstr::calculate_store_addr},
};
const uint32 FuncInstr::isaTableSize = countof(isaTable);

const char *FuncInstr::regTable[REG_NUM_MAX] =
{
    "zero",
    "at",
    "v0", "v1",
    "a0", "a1", "a2", "a3",
    "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7",
    "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7",
    "t8", "t9",
    "k0", "k1",
    "gp",
    "sp",
    "fp",
    "ra"
};

const char *FuncInstr::regTableName(RegNum reg) {
    return regTable[static_cast<size_t>(reg)];
}

FuncInstr::FuncInstr( uint32 bytes, uint32 PC) : instr(bytes), PC(PC)
{
    src1 = src2 = dst = REG_NUM_ZERO;
    complete = false;
    initFormat();
    switch ( format)
    {
        case FORMAT_R:
            initR();
            break;
        case FORMAT_I:
            initI();
            break;
        case FORMAT_J:
            initJ();
            break;
        case FORMAT_UNKNOWN:
            initUnknown();
            break;
    }
    new_PC = PC + 4;
}

std::string FuncInstr::Dump( std::string indent) const
{
    std::ostringstream oss;
    oss << indent << disasm;

    if ( dst != REG_NUM_ZERO && complete)
    {
        oss << "\t [ $" << regTableName(dst)
            << " = 0x" << std::hex << v_dst << "]" << std::dec;
    }
    return oss.str();
}

void FuncInstr::initFormat()
{
    bool is_R = ( instr.asR.opcode == 0x0);
    uint8 ident = is_R ? instr.asR.funct : instr.asR.opcode;

    for ( size_t i = 0; i < isaTableSize; i++)
    {
        bool is_i_R = ( isaTable[i].format == FORMAT_R);
        if ( isaTable[i].opcode == ident && ( is_i_R == is_R))
        {
            format    = isaTable[i].format;
            operation = isaTable[i].operation;
            mem_size  = isaTable[i].mem_size;
            isaNum    = i;
            if ( FORMAT_R == format)
               assert( instr.asR.opcode == 0x0);
            return;
        }
    }
    initUnknown();
}


void FuncInstr::initR()
{
    std::ostringstream oss;
    oss << isaTable[isaNum].name;
    switch ( operation)
    {
        case OUT_R_ARITHM:
            src2 = (RegNum)instr.asR.rt;
            src1 = (RegNum)instr.asR.rs;
            dst  = (RegNum)instr.asR.rd;

            oss <<  " $" << regTableName(dst)
                << ", $" << regTableName(src1)
                << ", $" << regTableName(src2);
            break;
        case OUT_R_SHAMT:
            src1  = (RegNum)instr.asR.rs;
            dst   = (RegNum)instr.asR.rd;
            v_imm = instr.asR.shamt;

            oss <<  " $" << regTableName(dst)
                << ", $" << regTableName(src1)
                <<  ", " << std::dec << v_imm;
            break;
        case OUT_R_JUMP_LINK:
            dst = REG_NUM_RA;
        case OUT_R_JUMP:
            src1  = (RegNum)instr.asR.rs;

            oss << " $" << regTableName(src1);
            break;
        case OUT_R_SPECIAL:
            break;
        default:
            assert(0);
    }
    disasm = oss.str();
}


void FuncInstr::initI()
{
    v_imm = instr.asI.imm;

    std::ostringstream oss;
    oss << isaTable[isaNum].name << " $";
    switch ( operation)
    {
        case OUT_I_ARITHM:
            src1 = (RegNum)instr.asI.rs;
            dst  = (RegNum)instr.asI.rt;

            oss << regTable[dst] << ", $"
                << regTable[src1] << ", "
                << std::hex << "0x" << v_imm << std::dec;

            break;
        case OUT_I_BRANCH:
            src1 = (RegNum)instr.asI.rs;
            src2 = (RegNum)instr.asI.rt;

            oss << regTable[src1] << ", $"
                << regTable[src2] << ", "
                << std::hex << "0x" << v_imm << std::dec;
            break;

        case OUT_I_CONST:
            dst  = (RegNum)instr.asI.rt;

            oss << regTable[dst] << std::hex
                << ", 0x" << v_imm << std::dec;
            break;

        case OUT_I_LOAD:
        case OUT_I_LOADU:
            src1 = (RegNum)instr.asI.rs;
            dst  = (RegNum)instr.asI.rt;

            oss << regTable[dst] << ", 0x"
                << std::hex << v_imm
                << "($" << regTable[src1] << ")" << std::dec;
            break;

        case OUT_I_STORE:
            src2 = (RegNum)instr.asI.rt;
            src1  = (RegNum)instr.asI.rs;
            dst  = REG_NUM_ZERO;

            oss << regTable[src2] << ", 0x"
                << std::hex << v_imm
                << "($" << regTable[src1] << ")" << std::dec;
            break;
        default:
            assert(0);
    }
    disasm = oss.str();
}

void FuncInstr::initJ()
{
    v_imm = instr.asJ.imm;

    std::ostringstream oss;
    oss << isaTable[isaNum].name << " "
        << std::hex << "0x" << v_imm;

    if ( operation == OUT_J_JUMP_LINK)
        dst = REG_NUM_RA;

    disasm = oss.str();
}

void FuncInstr::initUnknown()
{
    std::ostringstream oss;
    oss << std::hex << std::setfill( '0')
        << "0x" << std::setw( 8) << instr.raw << '\t' << "Unknown" << std::endl;
    disasm = oss.str();
    isaNum = 0;
}

void FuncInstr::execute_unknown()
{
    std::cerr << "ERROR.Incorrect instruction: " << disasm << std::endl;
    exit(EXIT_FAILURE);
}

std::ostream& operator<< ( std::ostream& out, const FuncInstr& instr)
{
    return out << instr.Dump( "");
}
