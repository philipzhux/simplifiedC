#include "symbol.h"

Symbol::Symbol(std::string name, bool isTerminal, int id) : humanReadableName(name), isTerminal(isTerminal), id(id){};


bool Symbol::operator==(const Symbol &other) const
{
    // optimize with id later
    return id == other.id;
}

NonTerminal::NonTerminal(std::string name, int id) : Symbol(name, false, id) {}


Terminal::Terminal(std::string name, Token token, int id) : Symbol(name, true, id), token(token) {}