/**
 * 
 *
 *
 */

#include "func_instr.h"
#include <sstream>

const char* registersTable[] =
{
    "$zero",
    "$at",
    "$v0", "$v1",
    "$a0", "$a1", "$a2", "$a3",
    "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7",
    "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7",
    "$t8", "$t9",
    "$k0", "$k1",        
    "$gp",
    "$sp",
    "$fp",
    "$ra"
};


int parse_command(std::string& str, Bytes bytes);

int undefined_command(std::string& str, Bytes bytes)
{
    std::ostringstream oss;
    oss<<"Undefined command\n";
    str = oss.str();
    return -1;
}
//---------------------------------------------- I TYPES
int addi(std::string& str, Bytes bytes) // 0x8
{
    std::ostringstream oss;
    oss << "addi ";
    oss << registersTable[ bytes.asI.t]<<", ";
    oss << registersTable[ bytes.asI.s]<<", ";
    oss << std::hex<<"0x"<< (int16) bytes.asI.imm << "\n";
    str = oss.str();
    return 0;
}
int addiu(std::string& str, Bytes bytes) // 0x9
{
    std::ostringstream oss;
    if ( bytes.asI.imm == 0 ) // move
    {
        oss << "move ";
        oss << registersTable[ bytes.asI.t]<<", ";
        oss << registersTable[ bytes.asI.s]<<"\n";
    }
    else if ( bytes.asI.s == 0)
    {
        oss << "li ";
        oss << registersTable[ bytes.asI.t]<<", ";
        oss << std::hex<<"0x"<<(uint16) bytes.asI.imm << "\n";
    }
    else
    {
        oss << "addiu ";
        oss << registersTable[ bytes.asI.t]<<", ";
        oss << registersTable[ bytes.asI.s]<<", ";
        oss << std::hex << (uint16) bytes.asI.imm << "\n";
    }
    str = oss.str();
    return 0;
}
int beq(std::string& str, Bytes bytes) // 0x4
{
    std::ostringstream oss;
    oss << "beq ";
    oss << registersTable[ bytes.asI.s]<<", ";
    oss << registersTable[ bytes.asI.t]<<", ";
    oss << std::hex<<"0x"<< (int16) bytes.asI.imm << "\n";
    str = oss.str();
    return 0;
}
int bne(std::string& str, Bytes bytes) // 0x5
{
    std::ostringstream oss;
    oss << "bne ";
    oss << registersTable[ bytes.asI.s]<<", ";
    oss << registersTable[ bytes.asI.t]<<", ";
    oss << std::hex<<"0x"<< (int16) bytes.asI.imm << "\n";
    str = oss.str();
    return 0;
}
//---------------------------------------------- R type

int parseR(std::string& str, Bytes bytes); // 0x0

int add(std::string& str, Bytes bytes) // 0x0 0x20
{
    std::ostringstream oss;
    oss << "add ";
    oss << registersTable[ bytes.asR.d]<<", ";
    oss << registersTable[ bytes.asR.s]<<", ";
    oss << registersTable[ bytes.asR.t]<<"\n";
    str = oss.str();
    return 0;
}
int addu(std::string& str, Bytes bytes) // 0x0 0x21
{
    std::ostringstream oss;
    if ( (bytes.asR.d == 0) && (bytes.asR.s == 0 ) )
    {
        oss << "clear ";
        oss << registersTable[ bytes.asR.t]<<"\n";
    }
    else
    {
        oss << "addu ";
        oss << registersTable[ bytes.asR.d]<<", ";
        oss << registersTable[ bytes.asR.s]<<", ";
        oss << registersTable[ bytes.asR.t]<<"\n";
    }
    str = oss.str();
    return 0;
}
int sub(std::string& str, Bytes bytes) // 0x0 0x22
{
    std::ostringstream oss;
    oss << "sub ";
    oss << registersTable[ bytes.asR.d]<<", ";
    oss << registersTable[ bytes.asR.s]<<", ";
    oss << registersTable[ bytes.asR.t]<<"\n";
    str = oss.str();
    return 0;
}
int subu(std::string& str, Bytes bytes) // 0x0 0x23
{
    std::ostringstream oss;
    oss << "subu ";
    oss << registersTable[ bytes.asR.d]<<", ";
    oss << registersTable[ bytes.asR.s]<<", ";
    oss << registersTable[ bytes.asR.t]<<"\n";
    str = oss.str();
    return 0;
}
int sll(std::string& str, Bytes bytes) // 0x0 0x0
{
    std::ostringstream oss;
    if ( bytes.raw == 0)
    {
        oss << "nop \n";
    }
    else
    {
        oss << "sll ";
        oss << registersTable[ bytes.asR.d]<<", ";
        oss << registersTable[ bytes.asR.t]<<", ";
        oss << bytes.asR.shamt<<"\n";
    }
    str = oss.str();
    return 0;
}
int srl(std::string& str, Bytes bytes) // 0x0 0x2
{
    std::ostringstream oss;
    oss << "srl ";
    oss << registersTable[ bytes.asR.d]<<", ";
    oss << registersTable[ bytes.asR.t]<<", ";
    oss << bytes.asR.shamt<<"\n";
    str = oss.str();
    return 0;
}
int jr(std::string& str, Bytes bytes) // 0x0 0x8
{
    std::ostringstream oss;
    oss << "jr ";
    oss << registersTable[ bytes.asR.s]<<"\n";
    str = oss.str();
    return 0;
}
//--------------- J type
int j(std::string& str, Bytes bytes) // 0x2
{
    std::ostringstream oss;
    oss << "j ";
    oss << bytes.asJ.address<<"\n";// !!!!!!!!???????!!!!!!
    str = oss.str();
    return 0;
}
// ADDITIONAL FUNCTIONS
int and_f(std::string& str, Bytes bytes) // 0x0 0x24
{
    std::ostringstream oss;
    oss << "and ";
    oss << registersTable[ bytes.asR.d]<<", ";
    oss << registersTable[ bytes.asR.s]<<", ";
    oss << registersTable[ bytes.asR.t]<<"\n";
    str = oss.str();
    return 0;
}
int or_f(std::string& str, Bytes bytes) // 0x0 0x25
{
    std::ostringstream oss;
    oss << "or ";
    oss << registersTable[ bytes.asR.d]<<", ";
    oss << registersTable[ bytes.asR.s]<<", ";
    oss << registersTable[ bytes.asR.t]<<"\n";
    str = oss.str();
    return 0;
}
int xor_f(std::string& str, Bytes bytes) // 0x0 0x26
{
    std::ostringstream oss;
    oss << "xor ";
    oss << registersTable[ bytes.asR.d]<<", ";
    oss << registersTable[ bytes.asR.s]<<", ";
    oss << registersTable[ bytes.asR.t]<<"\n";
    str = oss.str();
    return 0;
}
int nor_f(std::string& str, Bytes bytes) // 0x0 0x27
{
    std::ostringstream oss;
    oss << "nor ";
    oss << registersTable[ bytes.asR.d]<<", ";
    oss << registersTable[ bytes.asR.s]<<", ";
    oss << registersTable[ bytes.asR.t]<<"\n";
    str = oss.str();
    return 0;
}
int slt_f(std::string& str, Bytes bytes) // 0x0 0x2a
{
    std::ostringstream oss;
    oss << "slt ";
    oss << registersTable[ bytes.asR.d]<<", ";
    oss << registersTable[ bytes.asR.s]<<", ";
    oss << registersTable[ bytes.asR.t]<<"\n";
    str = oss.str();
    return 0;
}
int sllv_f(std::string& str, Bytes bytes) // 0x0 0x4
{
    std::ostringstream oss;
    oss << "sllv ";
    oss << registersTable[ bytes.asR.d]<<", ";
    oss << registersTable[ bytes.asR.t]<<", ";
    oss << registersTable[ bytes.asR.s]<<"\n";
    str = oss.str();
    return 0;
}
int srlv_f(std::string& str, Bytes bytes) // 0x0 0x6
{
    std::ostringstream oss;
    oss << "srlv ";
    oss << registersTable[ bytes.asR.d]<<", ";
    oss << registersTable[ bytes.asR.t]<<", ";
    oss << registersTable[ bytes.asR.s]<<"\n";
    str = oss.str();
    return 0;
}
int srav_f(std::string& str, Bytes bytes) // 0x0 0x7
{
    std::ostringstream oss;
    oss << "srav ";
    oss << registersTable[ bytes.asR.d]<<", ";
    oss << registersTable[ bytes.asR.t]<<", ";
    oss << registersTable[ bytes.asR.s]<<"\n";
    str = oss.str();
    return 0;
}
int lw_f(std::string& str, Bytes bytes) // 0x21
{
    std::ostringstream oss;
    oss << "lw ";
    oss << registersTable[ bytes.asI.t]<<", ";
    oss << std::hex<<"0x"<< (int16) bytes.asI.imm<<"(";
    oss << registersTable[ bytes.asI.s]<<")\n";
    str = oss.str();
    return 0;
}
int sw_f(std::string& str, Bytes bytes) // 0x2b
{
    std::ostringstream oss;
    oss << "sw ";
    oss << registersTable[ bytes.asI.t]<<", ";
    oss << std::hex<<"0x"<< (int16) bytes.asI.imm<<"(";
    oss << registersTable[ bytes.asI.s]<<")\n";
    str = oss.str();
    return 0;
}
int lui_f(std::string& str, Bytes bytes) // 0xf
{
    std::ostringstream oss;
    oss << "lui ";
    oss << registersTable[ bytes.asI.t]<<", ";
    oss << std::hex<<"0x"<< (uint16) bytes.asI.imm<<"\n";
    str = oss.str();
    return 0;
}
int syscall_f(std::string& str, Bytes bytes) // 0x0 0xc
{
    std::ostringstream oss;
    oss << "syscall \n";
    str = oss.str();
    return 0;
}
int jal_f(std::string& str, Bytes bytes) // 0x3
{
    std::ostringstream oss;
    oss << "jal ";
    oss << std::hex<<"0x"<<(((uint32) bytes.asJ.address)<<2)<<"\n";
    str = oss.str();
    return 0;
}
int multu_f(std::string& str, Bytes bytes) // 0x0 0x19
{
    std::ostringstream oss;
    oss << "multu ";
    oss << registersTable[ bytes.asR.s]<<", ";
    oss << registersTable[ bytes.asR.t]<<"\n";
    str = oss.str();
    return 0;
}
int mflo_f(std::string& str, Bytes bytes) // 0x0 0x12
{
    std::ostringstream oss;
    oss << "mflo ";
    oss << registersTable[ bytes.asR.d]<<"\n";
    str = oss.str();
    return 0;
}


int (*parse_command_hash[])(std::string& str, Bytes bytes) =
{
/* 0x00 */  parseR,
/* 0x01 */  undefined_command,
/* 0x02 */  j,
/* 0x03 */  jal_f,
/* 0x04 */  beq,
/* 0x05 */  bne,
/* 0x06 */  undefined_command,
/* 0x07 */  undefined_command,
/* 0x08 */  addi,
/* 0x09 */  addiu,
/* 0x0a */  undefined_command,
/* 0x0b */  undefined_command,
/* 0x0c */  undefined_command,
/* 0x0d */  undefined_command,
/* 0x0e */  undefined_command,
/* 0x0f */  lui_f,

/* 0x10 */  undefined_command,
/* 0x11 */  undefined_command,
/* 0x12 */  undefined_command,
/* 0x13 */  undefined_command,
/* 0x14 */  undefined_command,
/* 0x15 */  undefined_command,
/* 0x16 */  undefined_command,
/* 0x17 */  undefined_command,
/* 0x18 */  undefined_command,
/* 0x19 */  undefined_command,
/* 0x1a */  undefined_command,
/* 0x1b */  undefined_command,
/* 0x1c */  undefined_command,
/* 0x1d */  undefined_command,
/* 0x1e */  undefined_command,
/* 0x1f */  undefined_command,
     
/* 0x20 */  undefined_command,
/* 0x21 */  undefined_command,
/* 0x22 */  undefined_command,
/* 0x23 */  lw_f,
/* 0x24 */  undefined_command,
/* 0x25 */  undefined_command,
/* 0x26 */  undefined_command,
/* 0x27 */  undefined_command,
/* 0x28 */  undefined_command,
/* 0x29 */  undefined_command,
/* 0x2a */  undefined_command,
/* 0x2b */  sw_f,
/* 0x2c */  undefined_command,
/* 0x2d */  undefined_command,
/* 0x2e */  undefined_command,
/* 0x2f */  undefined_command,

/* 0x30 */  undefined_command,
/* 0x31 */  undefined_command,
/* 0x32 */  undefined_command,
/* 0x33 */  undefined_command,
/* 0x34 */  undefined_command,
/* 0x35 */  undefined_command,
/* 0x36 */  undefined_command,
/* 0x37 */  undefined_command,
/* 0x38 */  undefined_command,
/* 0x39 */  undefined_command,
/* 0x3a */  undefined_command,
/* 0x3b */  undefined_command,
/* 0x3c */  undefined_command,
/* 0x3d */  undefined_command,
/* 0x3e */  undefined_command,
/* 0x3f */  undefined_command

};


int (*parseR_hash[])(std::string& str, Bytes bytes) =
{
/* 0x00 */  sll,
/* 0x01 */  undefined_command,
/* 0x02 */  srl,
/* 0x03 */  undefined_command,
/* 0x04 */  sllv_f,
/* 0x05 */  undefined_command,
/* 0x06 */  srlv_f,
/* 0x07 */  srav_f,
/* 0x08 */  jr,
/* 0x09 */  undefined_command,
/* 0x0a */  undefined_command,
/* 0x0b */  undefined_command,
/* 0x0c */  syscall_f,
/* 0x0d */  undefined_command,
/* 0x0e */  undefined_command,
/* 0x0f */  undefined_command,

/* 0x10 */  undefined_command,
/* 0x11 */  undefined_command,
/* 0x12 */  mflo_f,
/* 0x13 */  undefined_command,
/* 0x14 */  undefined_command,
/* 0x15 */  undefined_command,
/* 0x16 */  undefined_command,
/* 0x17 */  undefined_command,
/* 0x18 */  undefined_command,
/* 0x19 */  multu_f,
/* 0x1a */  undefined_command,
/* 0x1b */  undefined_command,
/* 0x1c */  undefined_command,
/* 0x1d */  undefined_command,
/* 0x1e */  undefined_command,
/* 0x1f */  undefined_command,
     
/* 0x20 */  add,
/* 0x21 */  addu,
/* 0x22 */  sub,
/* 0x23 */  subu,
/* 0x24 */  and_f,
/* 0x25 */  or_f,
/* 0x26 */  xor_f,
/* 0x27 */  nor_f,
/* 0x28 */  undefined_command,
/* 0x29 */  undefined_command,
/* 0x2a */  slt_f,
/* 0x2b */  undefined_command,
/* 0x2c */  undefined_command,
/* 0x2d */  undefined_command,
/* 0x2e */  undefined_command,
/* 0x2f */  undefined_command,

/* 0x30 */  undefined_command,
/* 0x31 */  undefined_command,
/* 0x32 */  undefined_command,
/* 0x33 */  undefined_command,
/* 0x34 */  undefined_command,
/* 0x35 */  undefined_command,
/* 0x36 */  undefined_command,
/* 0x37 */  undefined_command,
/* 0x38 */  undefined_command,
/* 0x39 */  undefined_command,
/* 0x3a */  undefined_command,
/* 0x3b */  undefined_command,
/* 0x3c */  undefined_command,
/* 0x3d */  undefined_command,
/* 0x3e */  undefined_command,
/* 0x3f */  undefined_command

};


int parse_command(std::string& str, Bytes bytes)
{
    return parse_command_hash[bytes.asR.opcode](str,bytes);
}

int parseR(std::string& str, Bytes bytes) // 0x0
{
    return parseR_hash[bytes.asR.funct](str,bytes);
}

