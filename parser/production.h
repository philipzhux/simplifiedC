#pragma once
#include "symbol.h"
#include <vector>

/// @brief A production is a rule in the grammar.
/// @details The production is defined by a left-hand side symbol and a right-hand side sequence of symbols.
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