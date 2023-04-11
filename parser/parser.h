#pragma once
#include "configuration.h"


// add hash support for std::pair<int,int> type
namespace std
{
    template <>
    struct hash<std::pair<int, int>>
    {
        size_t operator()(const std::pair<int, int> &pair) const
        {
            size_t res = 17;
            res = res * 31 + hash<int>()(pair.first);
            res = res * 31 + hash<int>()(pair.second);
            return res;
        }
    };
}

/// @brief A parser is a set of productions, configuration sets, action table and goto table.
/// @details The parser is built by the build() method. The parse() method can be used to parse a sequence of symbols.
class Parser
{
public:
    Parser(Symbol startLhs, std::vector<Symbol> startRhs, Terminal endSymbol);
    Terminal endSymbol;
    int startProductionId;
    std::unordered_set<int> nullableSymbols;
    // production zero is the augmented production (goal)
    // reduce according to production zero is the accept state
    
    
    std::vector<Production> productions;
    std::vector<ConfigurationSet> configurationSets;
    // i>0: shift to state i; i=0: accept (reduce 0); i<0: reduce according to production -i
    std::unordered_map<std::pair<int, int>, int> actionTable;
    // only applied to non-terminal symbols and reduce actions
    std::unordered_map<std::pair<int, int>, int> gotoTable;
    void printProductions() const;
    void printConfigurationSets() const;
    void printConfigurationSet(const ConfigurationSet &configurationSet) const;
    void printActionTable() const;
    void printGotoTable() const;
    void build();
    bool parse(std::vector<Symbol> input);
    int addProduction(Symbol lhs, std::vector<Symbol> rhs);
    std::vector<Configuration> getClosure(const Configuration &configuration) const;
    std::vector<Symbol> getFirstSet(Symbol symbol) const;
    std::vector<Symbol> getFirstSet(const std::vector<Symbol> &symbols) const;

};


