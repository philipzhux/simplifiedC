#include "parser.h"
#include <iostream>
#include <map>
#include <cereal/archives/xml.hpp>
#include <fstream>

// namespace grammar
// {
//  Parser buildGrammar();
// }


class parserWrapper
{
    public:
    parserWrapper(std::string);
    Parser parser;
    std::map<Token, Symbol> tokenToTerminal;
    void buildGrammar();
};