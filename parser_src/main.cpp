#include "parser.h"
#include <iostream>
#include <map>
#include <cereal/archives/xml.hpp>
#include <fstream>
#include "grammar.h"
#include <stdlib.h>
inline bool exists_test(const std::string &name)
{
    std::ifstream f(name.c_str());
    return f.good();
}

int main(int argc, char **argv)
{
    // argv is the path to the parser state file
    // if(argc != 2)
    // {
    //     std::cout << "Usage: " << argv[0] << " <parser state file>" << std::endl;
    //     return 1;
    // }

    // std::string parserStatePath = argv[1];

    // if (!exists_test(parserStatePath))
    // {
    //     grammar::buildGrammar(parserStatePath);
    // }

    {
        // parser scope
        parserWrapper wrapper("");
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
                std::cout << "Scanner Syntax Error" << std::endl;
                exit(1);
            }

            
            for (auto &token : lex.matchedTokens)
            {
                auto symbol = wrapper.tokenToTerminal.at(token.token);
                switch (token.token)
                {
                    case Token::ID:
                    {
                        symbol.syntaxTreeNode = std::make_shared<ASTGen::SyntaxTreeNode>(ASTGen::ID,token.matchedString);
                        break;
                    }
                    case Token::INT_NUM:
                    {
                        symbol.syntaxTreeNode = std::make_shared<ASTGen::SyntaxTreeNode>(ASTGen::INTLITERAL,::atoi(token.matchedString.c_str()));
                        break;
                    }

                    default:
                    {
                        symbol.syntaxTreeNode = std::make_shared<ASTGen::SyntaxTreeNode>(ASTGen::RESERVED_WORDS);
                        break;
                    }

                }
                symbols.push_back(symbol);
            }
            
        }
        // wrapper.buildGrammar();
        wrapper.parser.parse(symbols);
        for (auto& code : wrapper.code.getAssembly())
        {
            std::cout << code << std::endl;
        }

    }
}
