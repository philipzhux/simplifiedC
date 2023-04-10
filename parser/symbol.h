#pragma once
#include "scanner.hpp"
#include "parser.h"
#include <memory>
#include <unordered_set>

class ConfigurationSet;
class Symbol
{
public:
    Symbol(std::string name, bool isTerminal, int id);
    bool operator==(const Symbol &other) const;
    bool isTerminal;
    std::string humanReadableName;
    static int getId()
    {
        static int id = 0;
        return id++;
    }

    int id;
};

class Terminal : public Symbol
{
public:
    Terminal(std::string name, Token token, int id);

private:
    Token token;
};

class NonTerminal : public Symbol
{
public:
    NonTerminal(std::string name, int id);
};

