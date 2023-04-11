#include "symbol.h"

Symbol::Symbol(std::string name, bool isTerminal, int id) : humanReadableName(name), isTerminal(isTerminal), id(id){};

Symbol::Symbol() : humanReadableName("INVALID"), isTerminal(false), id(-1){};

bool Symbol::operator==(const Symbol &other) const
{
    // optimize with id later
    return id == other.id;
}

int Symbol::getId()
{
    static int id = 0;
    return id++;
}

NonTerminal::NonTerminal(std::string name, int id) : Symbol(name, false, id) {}

Terminal::Terminal(std::string name, Token token, int id) : Symbol(name, true, id), token(token) {}

Terminal::Terminal() : Symbol("INVALID", true, -1), token(Token()) {}