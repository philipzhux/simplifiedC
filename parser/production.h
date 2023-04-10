#pragma once
#include "symbol.h"
#include <vector>

class Production
{
public:
    Production(Symbol lhs, std::vector<Symbol> rhs, int id);
    int id;
    Symbol lhs;
    std::vector<Symbol> rhs;
    bool operator==(const Production &other) const;
    static int getId();
};