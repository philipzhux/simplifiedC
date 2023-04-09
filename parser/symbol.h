#include "scanner.hpp"
#include <memory>
#include <unordered_set>

class Symbol
{
    // This is a base class for all symbols in the grammar.
public:
    Symbol(std::string name, bool isTerminal, int id) : humanReadableName(name), isTerminal(isTerminal), id(id){};
    bool operator==(const Symbol &other) const
    {
        // optimize with id later
        return id == other.id;
    }
    bool isTerminal;
    std::string humanReadableName;
    static int getId()
    {
        static int id = 0;
        return id++;
    }
    int id;
};

class Terminal : public Symbol
{
    // This is a class for terminal symbols.
public:
    Terminal(std::string name, Token token, int id) : Symbol(name, true, id), token(token) {}

private:
    Token token;
};

class NonTerminal : public Symbol
{
    // This is a class for non-terminal symbols.
public:
    NonTerminal(std::string name, int id) : Symbol(name, false, id) {}
};

class Production
{
    // This is a class for production rules.
public:
    Production(Symbol lhs, std::vector<Symbol> rhs, int id) : lhs(lhs), rhs(rhs), id(id) {
        assert(!lhs.isTerminal);
    }
    int id;
    Symbol lhs;
    std::vector<Symbol> rhs;
    bool operator==(const Production &other) const
    {
        return id==other.id;
    }
    static int getId()
    {
        static int id = 0;
        return id++;
    }
};

class Configuration
{
    // This is a class for configurations.
public:
    Configuration(std::shared_ptr<Production> production, int dotPosition, Symbol lookahead) : production(production), dotPosition(dotPosition), lookahead(lookahead) {}
    std::shared_ptr<Production> production;
    int dotPosition;
    Symbol lookahead;
    std::pair<Symbol, Configuration> getTransition()
    {
        assert(!isComplete());
        return std::make_pair(production->rhs[dotPosition], Configuration(production, dotPosition + 1, lookahead));
    }
    bool operator==(const Configuration &other) const
    {
        return *production == *other.production && dotPosition == other.dotPosition && lookahead == other.lookahead;
    }
    // This is a function for checking if the configuration is complete.
    bool isComplete()
    {
        return dotPosition == production->rhs.size();
    }
    // This is a function to get the symbol after dot.
    Symbol getSymbolAfterDot()
    {
        return production->rhs[dotPosition];
    }
    // This is a function to get the closure of the configuration.
    std::vector<Configuration> closure(const Parser &parser)
    {
        std::vector<Configuration> closure;
        closure.push_back(*this);
        bool changed = true;
        while (changed)
        {
            changed = false;
            for (auto configuration : closure)
            {
                if (configuration.isComplete())
                {
                    continue;
                }
                Symbol symbolAfterDot = configuration.getSymbolAfterDot();
                if (!symbolAfterDot.isTerminal)
                {
                    for (auto const &productionPair : parser.productionMap)
                    {
                        auto const &production = productionPair.second;
                        if (production.lhs == symbolAfterDot)
                        {
                            std::vector<Symbol> followSymbols;
                            for (int i = configuration.dotPosition + 1; i < configuration.production->rhs.size(); i++)
                            {
                                followSymbols.push_back(configuration.production->rhs[i]);
                            }
                            followSymbols.push_back(configuration.lookahead);
                            for (auto lookahead : parser.getFirstSet(followSymbols))
                            {
                                Configuration newConfiguration = Configuration(std::make_shared<Production>(production), 0, lookahead);

                                if (std::find(closure.begin(), closure.end(), newConfiguration) == closure.end())
                                {
                                    closure.push_back(newConfiguration);
                                    changed = true;
                                }
                            }
                        }
                    }
                }
            }
        }
        return closure;
    }
};

class ConfigurationSet
{
    // This is a class for configuration sets.
public:
    ConfigurationSet(std::vector<Configuration> configurations, int id) : configurations{configurations.begin(),configurations.end()}, id(id) {}
    static int getId()
    {
        static int id = 0;
        return id++;
    }
    bool operator==(const ConfigurationSet &other) const
    {
        return configurations == other.configurations;
    }
    std::unordered_set<Configuration> configurations;
    int id;
};

class Parser
{
    // This is a class for parser.
public:
    Parser(Production startProduction): startProduction(startProduction) {
        addProduction(startProduction);
    };
    Production startProduction;
    std::unordered_set<Symbol> nullableSymbols;
    std::unordered_map<int, Production> productionMap;
    // std::vector<Production> productions;
    std::vector<ConfigurationSet> configurationSets;
    std::unordered_map<std::pair<int, int>, int> actionTable;
    // go to table
    std::unordered_map<std::pair<int, int>, int> gotoTable;
   
    void build(Symbol endSymbol)
    {
        // build configuration sets and action table
        Configuration initialConfig = Configuration(std::make_shared<Production>(startProduction), 0, endSymbol);
        ConfigurationSet initialConfigurationSet = ConfigurationSet(initialConfig.closure(*this), ConfigurationSet::getId());
        configurationSets.push_back(initialConfigurationSet);
        while(true)
        {
            bool changed = false;
            for(auto configurationSet : configurationSets)
            {
                for(auto configuration : configurationSet.configurations)
                {
                    if(configuration.isComplete())
                    {
                        continue;
                    }
                    std::pair<Symbol, Configuration> transition = configuration.getTransition();
                    std::vector<Configuration> closure = transition.second.closure(*this);
                    ConfigurationSet newConfigurationSet = ConfigurationSet(closure, ConfigurationSet::getId());
                    if(std::find(configurationSets.begin(), configurationSets.end(), newConfigurationSet) == configurationSets.end())
                    {
                        configurationSets.push_back(newConfigurationSet);
                        changed = true;
                    }
                    actionTable[std::make_pair(configurationSet.id, transition.first.id)] = newConfigurationSet.id;
                }
            }
            if(!changed)
            {
                break;
            }
        }
        // now consider the reduction case
        for(auto configurationSet : configurationSets)
        {
            for(auto configuration : configurationSet.configurations)
            {
                if(configuration.isComplete())
                {
                    actionTable[std::make_pair(configurationSet.id, configuration.lookahead.id)] = -configuration.production->id;
                    // go to table
                    gotoTable[std::make_pair(configurationSet.id, configuration.production->lhs.id)] = configurationSet.id;
                }
            }
        }

    }

    // function to parse the input
    bool parse(std::vector<Symbol> input, Symbol endSymbol)
    {
        std::vector<int> stack;
        stack.push_back(0);
        input.push_back(endSymbol);
        int inputCursor = 0;
        while(true)
        {
            int currentState = stack.back();
            Symbol currentSymbol = input[inputCursor];
            if(actionTable.find(std::make_pair(currentState, currentSymbol.id)) == actionTable.end())
            {
                return false;
            }
            int nextState = actionTable[std::make_pair(currentState, currentSymbol.id)];
            if(nextState > 0)
            {
                stack.push_back(nextState);
                inputCursor++;
            }
            else if(nextState < 0)
            {
                // pop series of states from the stack according to the rhs of the production
                for(int j = 0; j < productionMap[-nextState].rhs.size(); j++)
                {
                    stack.pop_back();
                }
                // push the next state according to the goto table
                int nextState = gotoTable[std::make_pair(stack.back(), productionMap[-nextState].lhs.id)];
                stack.push_back(nextState);

            }
            else
            {
                return true;
            }
        }
    }

    



    void addProduction(Production production)
    {
        if(production.rhs.size() == 0)
        {
            nullableSymbols.insert(production.lhs);
        }
        productionMap[production.id] = production;
    }
    std::vector<Symbol> getFirstSet(Symbol symbol) const
    {
        std::vector<Symbol> firstSet;
        for (auto const & productionPair : productionMap)
        {
            auto const & production = productionPair.second;
            if (production.lhs == symbol)
            {
                if (production.rhs[0].isTerminal)
                {
                    firstSet.push_back(production.rhs[0]);
                }
                else
                {
                    std::vector<Symbol> firstSetOfRhs = getFirstSet(production.rhs[0]);
                    firstSet.insert(firstSet.end(), firstSetOfRhs.begin(), firstSetOfRhs.end());
                }
            }
        }
        return firstSet;
    }

    std::vector<Symbol> getFirstSet(const std::vector<Symbol> &symbols) const
    {
        std::vector<Symbol> firstSet;
        for (auto symbol : symbols)
        {
            std::vector<Symbol> firstSetOfSymbol = getFirstSet(symbol);
            firstSet.insert(firstSet.end(), firstSetOfSymbol.begin(), firstSetOfSymbol.end());
            if (nullableSymbols.count(symbol) == 0)
            {
                break;
            }
        }
        return firstSet;
    }
};
