#pragma once

#include <string>
#include <cereal/types/string.hpp>
#include <cereal/types/utility.hpp>
#include <vector>
#include <cereal/types/vector.hpp>
#include <unordered_set>
#include <cereal/types/unordered_set.hpp>
#include <unordered_map>
#include <cereal/types/unordered_map.hpp>
#include <memory>
#include <cereal/types/memory.hpp>
#include <cassert>
#include <algorithm>
#include "production.h"

/// @brief A configuration is a production with a dot position and a set of lookahead symbols.
class Configuration
{
public:
    Configuration();
    Configuration(std::shared_ptr<Production> production, int dotPosition, std::unordered_set<Symbol> lookaheads);
    std::shared_ptr<Production> production;
    int dotPosition;
     std::unordered_set<Symbol> lookaheads;
    std::pair<Symbol, Configuration> getTransition() const;
    bool operator==(const Configuration &other) const;
    bool isComplete() const;
    Symbol getSymbolAfterDot();
    // add hash support for class Configuration
    friend struct std::hash<Configuration>;

    template <class Archive>
    void serialize(Archive &archive)
    {
        // serialize things by passing them to the archive
        archive(production, dotPosition, lookaheads);
        
    }

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
             for(const auto& symbol : configuration.production->rhs)
                res = res * 31 + hash<int>()(symbol.id);
            res = res * 31 + hash<int>()(configuration.dotPosition);
            for(const auto& lookahead : configuration.lookaheads)
                res = res * 31 + hash<int>()(lookahead.id);
            return res;
        }
    };
}


/// @brief A configuration set is a set of configurations.
/// @details Each configuration set is a state in the LR(1) automaton.
class ConfigurationSet
{
public:
    ConfigurationSet();
    ConfigurationSet(std::unordered_set<Configuration> configurations, int id);
    static int controlId(bool increment);
    static int getId();
    static void rollbackId();
    bool operator==(const ConfigurationSet &other) const;
    std::unordered_set<Configuration> configurations;
    int id;

    template <class Archive>
    void serialize(Archive &archive)
    {
        // serialize things by passing them to the archive
        archive(configurations, id);
    }
};