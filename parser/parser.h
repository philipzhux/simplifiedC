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


class Parser
{
public:
    Parser(Symbol startLhs, std::vector<Symbol> startRhs, Terminal endSymbol);
    Terminal endSymbol;
    int startProductionId;
    std::unordered_set<int> nullableSymbols;
    std::vector<Production> productions;
    std::vector<ConfigurationSet> configurationSets;
    std::unordered_map<std::pair<int, int>, int> actionTable;
    std::unordered_map<std::pair<int, int>, int> gotoTable;
    void printProductions() const;
    void printConfigurationSets() const;
    void printActionTable() const;
    void printGotoTable() const;
    void build();
    bool parse(std::vector<Symbol> input);
    int addProduction(Symbol lhs, std::vector<Symbol> rhs);
    std::vector<Configuration> getClosure(const Configuration &configuration) const;
    std::vector<Symbol> getFirstSet(Symbol symbol) const;
    std::vector<Symbol> getFirstSet(const std::vector<Symbol> &symbols) const;

};


