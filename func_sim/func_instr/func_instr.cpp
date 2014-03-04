/**
 * implementation of class FuncInstr
 *
 *
 */


#include "func_instr.h"
#include <sstream>

FuncInstr::FuncInstr( uint32 bytes)
{
printf("Constryctor\n");
    this->initFormat( bytes);
    switch ( this->format)
    {
        case FORMAT_R:
            this->parseR( );
            break;
        case FORMAT_J:
            this->parseJ( );
            break;
        case FORMAT_I:
            this->parseI( );
            break;
        default:
            std::ostringstream oss;
            oss<<"Undefined command\n";
            this->textAss = oss.str();
    }
}

int FuncInstr::initFormat( uint32 bytes)
{
printf("initFormat\n");
    this->bytes.raw = bytes;
    uint8 opcode = this->bytes.asR.opcode;
printf("%llx\n",(uint64)bytes);
printf("%x %x %x %x %x\n",
    (uint32) this->bytes.asR.opcode, 
    (uint32) this->bytes.asR.t, 
    (uint32) this->bytes.asR.s, 
    (uint32) this->bytes.asR.d,
    (uint32) this->bytes.asR.funct); 
    for( int i = 0;
      this->isaTable[i].type != END_OF_INSTRUCTIONS_SET;
      i++)
    {
        // if we can identify only on opcode!
        if ( opcode == this->isaTable[i].opcode)
        {
            this->format = this->isaTable[i].format;
            this->type   = this->isaTable[i].type;
            return 0;
        }
    }
    return -1;
}

int FuncInstr::parseR()
{
printf("r\n");
    uint8 opcode = this->bytes.asR.opcode;
    uint8 funct = this->bytes.asR.funct;
    uint32 bytes = this->bytes.raw;
    std::ostringstream oss;
    int i = 0;
    while(
     !( opcode== this->isaTable[i].opcode && funct== this->isaTable[i].func)
     && 
     (this->isaTable[i].type!= END_OF_INSTRUCTIONS_SET) )
        i++;

    if ( bytes == 0)                     //------- nop
    {
        oss<<"nop\n";
        this->textAss = oss.str();
        return 0;
    }
    else if( !opcode && (funct==0x21) &&  //------- clear
             !this->bytes.asR.s && !this->bytes.asR.t )
    {
        oss<<"clear ";
        oss << this->registersTable[this->bytes.asR.d];
        this->textAss = oss.str();
        return 0;
    }
    this->type = this->isaTable[i].type;
    //-------------- name %1 %2 %3
    if( this->type == ADD || this->type == SUB)
    {
        oss<<this->isaTable[i].name;
        oss<<this->registersTable[this->bytes.asR.d];
        oss<<this->registersTable[this->bytes.asR.s];
        oss<<this->registersTable[this->bytes.asR.t]<<"\n";
        this->textAss = oss.str();
        return 0;
    }
    //-------------- name %1 %2 C
    if( this->type == SHIFT)
    {
        oss<< this->isaTable[i].name<< " ";
        oss<<this->registersTable[this->bytes.asR.d];
        oss<<this->registersTable[this->bytes.asR.t];
        oss<<(uint32) this->bytes.asR.shamt<<"\n";
        this->textAss = oss.str();
        return 0;
    }
    return -1;
}

int FuncInstr::parseI()
{
printf("I\n");
    uint8 opcode = this->bytes.asR.opcode;
    uint32 bytes = this->bytes.raw;
    std::ostringstream oss;
    int i = 0;
    while(
     !( opcode== this->isaTable[i].opcode)
     && 
     (this->isaTable[i].type!= END_OF_INSTRUCTIONS_SET) ) 
        i++;
printf("1\n");
    if ( (opcode == 0x9) && !this->bytes.asI.imm)//----move
    {
        oss<< "move "<< this->registersTable[this->bytes.asI.s];
        oss<< this->registersTable[this->bytes.asI.t]<<"\n";
        this->textAss = oss.str();
        return 0;
    }
printf("2\n");
    if ( this->type == BRANCH
         || 
         this->type == ADD )
    {
        oss<<this->isaTable[i].name<<" ";
        oss<<this->registersTable[this->bytes.asI.s];
        oss<<this->registersTable[this->bytes.asI.t];
        oss<<(int16) this->bytes.asI.imm<< "\n";//?????????????????????
        this->textAss = oss.str();
        return 0;
    }
    if ( this->type == ADDU)
    {
        oss<<this->isaTable[i].name<<" ";
        oss<<this->registersTable[this->bytes.asI.s];
        oss<<this->registersTable[this->bytes.asI.t];
        oss<<(uint16) this->bytes.asI.imm<<"\n";///??????????????????????????
        this->textAss = oss.str();
        return 0;
    }
printf("3\n");
    return -1;
}

int FuncInstr::parseJ()
{
printf("J\n");
    uint8 opcode = this->bytes.asR.opcode;
    uint32 bytes = this->bytes.raw;
    std::ostringstream oss;
    int i = 0;
    while(
     !( opcode== this->isaTable[i].opcode)
     && 
     (this->isaTable[i].type!= END_OF_INSTRUCTIONS_SET) )
        i++;
    
    if ( this->type == BRANCH)
    {
        oss<<this->isaTable[i].name<<"\n";
        oss<<(uint32) this->bytes.asJ.address;
        this->textAss = oss.str();
        return 0;
    }
    return -1;
};

std::string FuncInstr::Dump( std::string indent) const
{
    return indent+this->textAss;
};

std::ostream& operator<< ( std::ostream& out, const FuncInstr& instr)
{
    return out << instr.Dump("");
};



const FuncInstr::ISAEntry FuncInstr::isaTable[] = 
{
    // name   opcode    func  format               type
    { "add  ", 0x0,     0x20, FuncInstr::FORMAT_R, FuncInstr::ADD    },
    { "addu " , 0x0,     0x21, FuncInstr::FORMAT_R, FuncInstr::ADD    },
    { "sub "  , 0x0,     0x22, FuncInstr::FORMAT_R, FuncInstr::SUB    },
    { "subu " , 0x0,     0x23, FuncInstr::FORMAT_R, FuncInstr::SUB    },
    { "addi " , 0x8,     0x00, FuncInstr::FORMAT_I, FuncInstr::ADD    },
    { "addiu ", 0x9,     0x00, FuncInstr::FORMAT_I, FuncInstr::ADDU    },
    { "sll "  , 0x0,     0x00, FuncInstr::FORMAT_R, FuncInstr::SHIFT  },
    { "srl "  , 0x0,     0x02, FuncInstr::FORMAT_R, FuncInstr::SHIFT  },
    { "beq "  , 0x4,     0x00, FuncInstr::FORMAT_I, FuncInstr::BRANCH },
    { "bne "  , 0x5,     0x00, FuncInstr::FORMAT_I, FuncInstr::BRANCH },
    { "j "    , 0x2,     0x00, FuncInstr::FORMAT_J, FuncInstr::BRANCH },
    { "jr "   , 0x0,     0x08, FuncInstr::FORMAT_R, FuncInstr::BRANCH },
    // the last one must be of "END_OF..." type
    { "",0x0,0x0,FuncInstr::FORMAT_R ,FuncInstr::END_OF_INSTRUCTIONS_SET}

};

const char* FuncInstr::registersTable[] =
{
    "$zero ",
    "$at ",
    "$v0 ", "$v1 ",
    "$a0 ", "$a2 ", "$a3 ",
    "$t0 ", "$t1 ", "$t2 ", "$t3 ", "$t4 ", "$t5 ", "$t6 ", "$t7 ",
    "$s0 ", "$s1 ", "$s2 ", "$s3 ", "$s4 ", "$s5 ", "$s6 ", "$s7 ",
    "$t8 ", "$t9 ",
    "$k0 ", "$k1 ",        
    "$gp ",
    "$sp ",
    "$fp ",
    "$ra "
};




