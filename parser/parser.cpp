#include "parser.h"
#include "configuration.h"

void Parser::build()
{
    // build configuration sets and action table
    Configuration initialConfig = Configuration(std::make_shared<Production>(startProduction), 0, endSymbol);
    ConfigurationSet initialConfigurationSet = ConfigurationSet(initialConfig.closure(*this), ConfigurationSet::getId());
    configurationSets.push_back(initialConfigurationSet);
    while (true)
    {
        bool changed = false;
        for (auto configurationSet : configurationSets)
        {
            for (auto configuration : configurationSet.configurations)
            {
                if (configuration.isComplete())
                {
                    continue;
                }
                std::pair<Symbol, Configuration> transition = configuration.getTransition();
                std::vector<Configuration> closure = transition.second.closure(*this);
                ConfigurationSet newConfigurationSet = ConfigurationSet(closure, ConfigurationSet::getId());
                if (std::find(configurationSets.begin(), configurationSets.end(), newConfigurationSet) == configurationSets.end())
                {
                    configurationSets.push_back(newConfigurationSet);
                    changed = true;
                }
                actionTable[std::make_pair(configurationSet.id, transition.first.id)] = newConfigurationSet.id;
            }
        }
        if (!changed)
        {
            break;
        }
    }
    // now consider the reduction case
    for (auto configurationSet : configurationSets)
    {
        for (auto configuration : configurationSet.configurations)
        {
            if (configuration.isComplete())
            {
                actionTable[std::make_pair(configurationSet.id, configuration.lookahead.id)] = -configuration.production->id;
                // go to table
                gotoTable[std::make_pair(configurationSet.id, configuration.production->lhs.id)] = configurationSet.id;
            }
        }
    }
}

// function to parse the input
bool Parser::parse(std::vector<Symbol> input)
{
    std::vector<int> stack;
    stack.push_back(0);
    input.push_back(endSymbol);
    int inputCursor = 0;
    while (true)
    {
        int currentState = stack.back();
        Symbol currentSymbol = input[inputCursor];
        if (actionTable.find(std::make_pair(currentState, currentSymbol.id)) == actionTable.end())
        {
            return false;
        }
        int nextState = actionTable[std::make_pair(currentState, currentSymbol.id)];
        if (nextState > 0)
        {
            stack.push_back(nextState);
            inputCursor++;
        }
        else if (nextState < 0)
        {
            // pop series of states from the stack according to the rhs of the production
            for (int j = 0; j < productionMap[-nextState].rhs.size(); j++)
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

void Parser::addProduction(Production production)
{
    if (production.rhs.size() == 0)
    {
        nullableSymbols.insert(production.lhs);
    }
    productionMap[production.id] = production;
}

std::vector<Symbol> Parser::getFirstSet(Symbol symbol) const
{
    std::vector<Symbol> firstSet;
    for (auto const &productionPair : productionMap)
    {
        auto const &production = productionPair.second;
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

std::vector<Symbol> Parser::getFirstSet(const std::vector<Symbol> &symbols) const
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