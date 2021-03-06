// Generci C
#include <string.h>
#include <stdlib.h>
#include <assert.h>

// Generic C++
#include <iostream>

// uArchSim modules
#include "elf_parser.h"

int main ( int argc, char* argv[])
{
    const int num_of_args = 2;
    
    if ( argc == num_of_args && strcmp( argv[ 1],"--help"))
    {
        // extract all ELF sections into the section_array variable
        std::vector<ElfSection> sections_array;
        ElfSection::getAllElfSections( argv[1], sections_array);
        
        // print the information about each section
        for ( const auto& section : sections_array)
	     std::cout << section.dump() << std::endl;
 
    } else if ( argc != num_of_args)
    {
        std::cerr << "ERROR: wrong number of arguments!" << std::endl
                  << "Type \"" << argv[ 0]
                  << " --help\" for usage." << std::endl;
        exit( EXIT_FAILURE);

    } else
    {
        std::cout << "This program prints content of all the sections" << std::endl
                  << "of the ELF binary file, which name is given as only parameter." << std::endl
                  << std::endl
                  << "Usage: \"" << argv[ 0] << " <ELF binary file>\"" << std::endl; 
    }

    return 0;
}
