#pragma once
#include "scanner.hpp"
#include <memory>
#include <unordered_set>

class Symbol
{
public:
    Symbol(std::string name, bool isTerminal, int id);
    bool operator==(const Symbol &other) const;
    bool isTerminal;
    std::string humanReadableName;
    static int getId();
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

