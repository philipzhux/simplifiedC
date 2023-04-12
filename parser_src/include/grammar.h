#include "parser.h"
#include <iostream>
#include <map>
#include <cereal/archives/xml.hpp>
#include <fstream>

namespace grammar
{
 size_t buildGrammar(std::string);
}


class parserWrapper
{
    public:
    parserWrapper(std::string);
    Parser parser;
    std::map<Token, Symbol> tokenToTerminal;
};