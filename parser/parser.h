#pragma once
#include "configurationSet.h"
#include "configuration.h"
#include "symbol.h"

class Parser
{
public:
    Parser(Production startProduction, Terminal endSymbol);
    Terminal endSymbol;
    Production startProduction;
    std::unordered_set<Symbol> nullableSymbols;
    std::unordered_map<int, Production> productionMap;
    std::vector<ConfigurationSet> configurationSets;
    std::unordered_map<std::pair<int, int>, int> actionTable;
    std::unordered_map<std::pair<int, int>, int> gotoTable;

    void build();
    bool parse(std::vector<Symbol> input);
    void addProduction(Production production);
    std::vector<Symbol> getFirstSet(Symbol symbol) const;
    std::vector<Symbol> getFirstSet(const std::vector<Symbol> &symbols) const;
};