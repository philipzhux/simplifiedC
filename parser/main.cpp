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

int main()
{
    std::string parserStatePath = "parser.xml";

    if (!exists_test(parserStatePath))
    {
        grammar::buildGrammar(parserStatePath);
    }

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

// NonTerminal _sPrime("S'", Symbol::getId());
// NonTerminal _s("S", Symbol::getId());
// NonTerminal _x("X", Symbol::getId());
// Terminal _$("$", $, Symbol::getId());
// Terminal _a("a", A, Symbol::getId());
// Terminal _b("b", B, Symbol::getId());

// Parser parser(_sPrime, {_s}, _$);
// parser.addProduction(_s, {_x, _x});
// parser.addProduction(_x, {_a, _x});
// parser.addProduction(_x, {_b});
// parser.build();
// parser.printProductions();
// parser.printConfigurationSets();
// parser.printActionTable();
// parser.printGotoTable();