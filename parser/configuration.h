#pragma once

#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <cassert>
#include <algorithm>
#include "production.h"
#include "symbol.h"

class Configuration
{
public:
    Configuration(std::shared_ptr<Production> production, int dotPosition, Symbol lookahead);
    std::shared_ptr<Production> production;
    int dotPosition;
    Symbol lookahead;
    std::pair<Symbol, Configuration> getTransition();
    bool operator==(const Configuration &other) const;
    bool isComplete();
    Symbol getSymbolAfterDot();
    std::vector<Configuration> closure(const Parser &parser);
};