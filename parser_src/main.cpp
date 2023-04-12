#include "parser.h"
#include <iostream>
#include <map>
#include <cereal/archives/xml.hpp>
#include <fstream>
#include "grammar.h"

inline bool exists_test(const std::string &name)
{
    std::ifstream f(name.c_str());
    return f.good();
}

int main(int argc, char **argv)
{
    // argv is the path to the parser state file
    if(argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " <parser state file>" << std::endl;
        return 1;
    }

    std::string parserStatePath = argv[1];

    // if (!exists_test(parserStatePath))
    // {
    //     grammar::buildGrammar(parserStatePath);
    // }

    {
        // parser scope
        parserWrapper wrapper(parserStatePath);
        std::vector<Symbol> symbols;
        
        {
            // scanner scope
            std::string src = "";
            for (std::string line; std::getline(std::cin, line);)
            {
                src += line + "\n";
            }
            Scanner lex(src);
            if (!lex.compile())
            {
                std::cout << "Syntax Error" << std::endl;
            }

            
            for (auto &token : lex.matchedTokens)
            {
                symbols.push_back(wrapper.tokenToTerminal.at(token));
            }
            
        }

        wrapper.parser.parse(symbols);
    }
}
