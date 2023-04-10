#include "parser.h"
#include <iostream>

Parser::Parser(Symbol startLhs, std::vector<Symbol> startRhs, Terminal endSymbol): endSymbol(endSymbol)
{
    startProductionId = addProduction(startLhs, startRhs);
}



std::vector<Configuration> Parser::getClosure(const Configuration &configuration) const
{
    std::vector<Configuration> closure;
    closure.push_back(configuration);
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
                for (auto const &production : productions)
                {
                    if (production.lhs == symbolAfterDot)
                    {
                        std::vector<Symbol> followSymbols;
                        for (int i = configuration.dotPosition + 1; i < configuration.production->rhs.size(); i++)
                        {
                            followSymbols.push_back(configuration.production->rhs[i]);
                        }
                        followSymbols.push_back(configuration.lookahead);
                        for (auto lookahead : getFirstSet(followSymbols))
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




void Parser::build()
{
    // build configuration sets and action table
    Configuration initialConfig = Configuration(std::make_shared<Production>(productions[startProductionId]), 0, endSymbol);
    ConfigurationSet initialConfigurationSet = ConfigurationSet(getClosure(initialConfig), ConfigurationSet::getId());
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
                std::vector<Configuration> closure = getClosure(transition.second);
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
            for (int j = 0; j < productions[-nextState].rhs.size(); j++)
            {
                stack.pop_back();
            }
            // push the next state according to the goto table
            int nextState = gotoTable[std::make_pair(stack.back(), productions[-nextState].lhs.id)];
            stack.push_back(nextState);
        }
        else
        {
            return true;
        }
    }
}

int Parser::addProduction(Symbol lhs, std::vector<Symbol> rhs)
{
    Production production = Production(lhs, rhs, productions.size());
    if (production.rhs.size() == 0)
    {
        nullableSymbols.insert(production.lhs.id);
    }
    productions.push_back(production);
    return production.id;
}

std::vector<Symbol> Parser::getFirstSet(Symbol symbol) const
{
    std::vector<Symbol> firstSet;
    for (auto const &production : productions)
    {
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
        if (nullableSymbols.count(symbol.id) == 0)
        {
            break;
        }
    }
    return firstSet;
}

void Parser::printProductions() const
{
    for (auto const &production : productions)
    {
        std::cout << production.lhs.humanReadableName << " -> ";
        for (auto const &symbol : production.rhs)
        {
            std::cout << symbol.humanReadableName << " ";
        }
        std::cout << std::endl;
    }
}

void Parser::printConfigurationSets() const
{
    for (auto const &configurationSet : configurationSets)
    {
        std::cout << "Configuration Set " << configurationSet.id << std::endl;
        for (auto const &configuration : configurationSet.configurations)
        {
            std::cout << configuration.production->lhs.humanReadableName << " -> ";
            for (int i = 0; i < configuration.production->rhs.size(); i++)
            {
                if (i == configuration.dotPosition)
                {
                    std::cout << ". ";
                }
                std::cout << configuration.production->rhs[i].humanReadableName << " ";
            }
            if (configuration.dotPosition == configuration.production->rhs.size())
            {
                std::cout << ".";
            }
            std::cout << ", " << configuration.lookahead.humanReadableName << std::endl;
        }
    }
}

void Parser::printActionTable() const
{
    std::cout << "Action Table" << std::endl;
    for (auto const &entry : actionTable)
    {
        std::cout << entry.first.first << " " << entry.first.second << " " << entry.second << std::endl;
    }
}

void Parser::printGotoTable() const
{
    std::cout << "Goto Table" << std::endl;
    for (auto const &entry : gotoTable)
    {
        std::cout << entry.first.first << " " << entry.first.second << " " << entry.second << std::endl;
    }
}