#include "grammar.h"
#include <iostream>

int main(int argc, char **argv)
{
    // argv is the path to the parser state file
    if(argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " <parser state file>" << std::endl;
        return 1;
    }
    std::cout << "Building parser..." << std::endl;
    size_t stateNum = grammar::buildGrammar(argv[1]);
    std::cout << "Parser built successfully ("<< stateNum <<" states), internal state exported to " << argv[1] << std::endl;

}