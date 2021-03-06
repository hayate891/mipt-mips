/**
 * elf_parser.h - Header of ELF binary perser
 * @author Alexander Titov <alexander.igorevich.titov@gmail.com>
 * Copyright 2013 MIPT-MIPS iLab project
 */

// protection from multi-include
#ifndef ELF_PARSER__ELF_PARSER_H
#define ELF_PARSER__ELF_PARSER_H

// Generic C++
#include <string>
#include <vector>

// uArchSim modules
#include <common/types.h>

struct Elf;

class ElfSection
{
    // You cannot use this constructor to create an object.
    // Use the static function getAllElfSections.
    ElfSection(); 
    ElfSection( const char* name, uint64 start_addr,
                uint64 size, const uint8* content);

public:
    const std::string name; // name of the elf section (e.g. ".text", ".data", etc)
    uint64 size; // size of the section in bytes
    uint64 start_addr; // the start address of the section
    uint8* content; // the row data of the section

    ElfSection( const  ElfSection& old);
    
    // Use this function to extract all sections from the ELF binary file.
    // Note that the 2nd parameter is used as output.
    static void getAllElfSections( const char* elf_file_name,
                                   std::vector<ElfSection>& sections_array /*used as output*/);
    
    virtual ~ElfSection();
    
    std::string dump( std::string indent = "") const;
    std::string strByBytes() const;
    std::string strByWords() const;
};

#endif // #ifndef ELF_PARSER__ELF_PARSER_H
