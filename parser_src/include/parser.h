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
    Parser();
    Parser(Symbol startLhs, std::vector<Symbol> startRhs, Terminal endSymbol);
    Terminal endSymbol;
    int startProductionId;
    int repCount;
    // track the path of recursion in building first set to avoid infinite recursion
    std::unordered_set<Symbol> firstSetRecursionMemo;
    std::unordered_map<Symbol, std::unordered_set<Symbol>> firstSetMemo;
    std::unordered_set<int> nullableSymbols;
    // production zero is the augmented production (goal)
    // reduce according to production zero is the accept state

    std::vector<std::shared_ptr<Production>> productions;
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
    void build(bool verbose);
    bool parse(std::vector<Symbol> input);
    bool parse(std::vector<Symbol> input, bool verbose);
    int addProduction(Symbol lhs, std::vector<Symbol> rhs, std::function<std::shared_ptr<ASTGen::SyntaxTreeNode>(std::vector<std::shared_ptr<ASTGen::SyntaxTreeNode>>)> action);
    std::vector<Configuration> getClosure(const Configuration &configuration);
    std::unordered_set<Symbol> getFirstSet(Symbol symbol);
    std::unordered_set<Symbol> getFirstSet(const std::vector<Symbol> &symbols);

    template <class Archive>
    void serialize(Archive &archive)
    {
        // serialize things by passing them to the archive
        archive(CEREAL_NVP(endSymbol), CEREAL_NVP(startProductionId), CEREAL_NVP(repCount),
         CEREAL_NVP(firstSetRecursionMemo), CEREAL_NVP(nullableSymbols), CEREAL_NVP(productions),
          CEREAL_NVP(configurationSets), CEREAL_NVP(actionTable), CEREAL_NVP(gotoTable));

        
    }
};
