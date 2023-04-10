#pragma once

#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <cassert>
#include <algorithm>
#include "production.h"

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
    // add hash support for class Configuration
    friend struct std::hash<Configuration>;
};

// implement hash function for class Configuration
namespace std
{
    template <>
    struct hash<Configuration>
    {
        size_t operator()(const Configuration &configuration) const
        {
            size_t res = 17;
            res = res * 31 + hash<int>()( configuration.production->lhs.id);
            res = res * 31 + hash<int>()( configuration.production->rhs[configuration.dotPosition].id);
            res = res * 31 + hash<int>()( configuration.lookahead.id);
            return res;
        }
    };
}


class ConfigurationSet
{
public:
    ConfigurationSet(std::vector<Configuration> configurations, int id);
    static int getId();
    bool operator==(const ConfigurationSet &other) const;
    std::unordered_set<Configuration> configurations;
    int id;
};