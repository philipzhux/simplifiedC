#include "parser.h"
#include <iostream>

Parser::Parser() {}

Parser::Parser(Symbol startLhs, std::vector<Symbol> startRhs, Terminal endSymbol) : endSymbol(endSymbol)
{
    startProductionId = addProduction(startLhs, startRhs);
    // augmented production should be the production 0
    // reduced by production 0 is the accept state
    assert(startProductionId == 0);
    repCount = 0;
}

/// @brief get the closure of a configuration
/// @param initConfiguration the initial configuration
/// @return a vector of configurations that are in the closure of initConfiguration
std::vector<Configuration> Parser::getClosure(const Configuration &initConfiguration)
{
    std::vector<Configuration> closure;
    bool added_config = true;
    bool modified_config = true;
    size_t i = 0;
    size_t modified_point = 0;
    closure.push_back(initConfiguration);
    while (added_config||modified_config) // keep expanding until no new configuration is added or modified
    {
        // by default, start from newly added configurations
        // if the closure is modified, then start from the modified point
        if (modified_config && i > modified_point) i = modified_point;
        size_t size = closure.size();
        added_config = false;
        modified_config = false;
        modified_point = size;
       
        for (; i < size; i++)
        {
            if (closure[i].isComplete())
            {
                continue;
            }
            Symbol symbolAfterDot = closure[i].getSymbolAfterDot();
            if (!symbolAfterDot.isTerminal)
            {
                for (const auto &production : productions)
                {
                    if (production->lhs == symbolAfterDot)
                    {
                        // found a production with same lhs, insert it into the closure
                        // expanding symbolAfterDot
                        for (const auto &configLookahead : closure[i].lookaheads)
                        {
                            // calculate the lookahead for the new configuration
                            std::vector<Symbol> followSymbols;

                            // the symbols after the expanded symbol, needed to construct the lookahead
                            for (int rhsIdx = closure[i].dotPosition + 1; rhsIdx < int(closure[i].production->rhs.size()); rhsIdx++)
                            {
                                followSymbols.push_back(closure[i].production->rhs[rhsIdx]);
                            }
                            followSymbols.push_back(configLookahead);

                            // lookahead is the first set of [the symbols after the expanded symbol] + [the lookahead of the original configuration]
                            for (const auto &lookahead : getFirstSet(followSymbols))
                            {
                                // flag to indicate whether the new configuration is already in the closure
                                // after the loop, if it is still true, then the new configuration is not in the closure
                                bool needInsert = true;
                                for (size_t j = 0; j < closure.size(); j++)
                                {
                                    if (*closure[j].production == *production && closure[j].dotPosition == 0)
                                    {
                                        // found a configuration with same production and dot position
                                        // may either add the lookahead to the existing configuration, or ignore it
                                        // if the lookahead is already in the existing configuration
                                        needInsert = false;
                                        if (closure[j].lookaheads.count(lookahead) == 0)
                                        {
                                            // found a configuration with same production and dot position, but different lookahead
                                            // merge the lookaheads
                                            closure[j].lookaheads.insert(lookahead);
                                            if(j<size) {
                                                // if the configuration is newly added, it is not counted as modified
                                                // otherwise, it is modified
                                                modified_config = true;
                                                // move the modified point to the leftmost modified configuration
                                                if(j<modified_point) modified_point = j;
                                            }
                                        }

                                        // at this point the new configuration is already in the closure
                                        // no need to insert it, so break the loop
                                        break;
                                    }
                                }
                                if (needInsert)
                                {
                                    // insert the new configuration
                                    closure.push_back({production, 0, {lookahead}});
                                    added_config = true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return closure;
}

/// @brief build the parser
/// @details build the parser by constructing the configuration sets, action table and goto table
void Parser::build()
{
    // build configuration sets and action table
    Configuration initialConfig = Configuration(productions[startProductionId], 0, {endSymbol});
    std::vector<Configuration> initialClosure = getClosure(initialConfig);
    ConfigurationSet initialConfigurationSet = ConfigurationSet({initialClosure.begin(), initialClosure.end()}, ConfigurationSet::getId());
    configurationSets.push_back(initialConfigurationSet);
    size_t i = 0;
    while (true)
    {
        bool changed = false;
        size_t size = configurationSets.size();
        for (; i < size; i++)
        {
            // auto configurationSet = configurationSets[i];
            std::unordered_map<Symbol, std::unordered_set<Configuration>> transitionMap;
            for (const auto &configuration : configurationSets[i].configurations)
            {
                if (configuration.isComplete())
                {
                    continue;
                }
                std::pair<Symbol, Configuration> transition = configuration.getTransition();
                std::vector<Configuration> closure = getClosure(transition.second);
                transitionMap[transition.first].insert(closure.begin(), closure.end());
            }
            for (const auto &transition : transitionMap)
            {
                ConfigurationSet newConfigurationSet = ConfigurationSet(transition.second, ConfigurationSet::getId());
                auto found = std::find(configurationSets.begin(), configurationSets.end(), newConfigurationSet);
                if (found == configurationSets.end())
                {
                    configurationSets.push_back(newConfigurationSet);
                    // std::cout<<"new configuration set: "<<newConfigurationSet.id<<" rep ="<< repCount << std::endl;
                    // printConfigurationSet(newConfigurationSet);
                    // changed = true;
                }
                else
                {
                    newConfigurationSet = *found;
                    ConfigurationSet::rollbackId();
                    repCount++;
                }
                std::unordered_map<std::pair<int, int>, int> &targetTable = transition.first.isTerminal ? actionTable : gotoTable;
                // printActionTable();
                // printGotoTable();
                if (targetTable.count(std::make_pair(configurationSets[i].id, transition.first.id)) == 0)
                {
                    changed = true;
                    targetTable[std::make_pair(configurationSets[i].id, transition.first.id)] = newConfigurationSet.id;
                }
                else
                {
                    if (targetTable[std::make_pair(configurationSets[i].id, transition.first.id)] != newConfigurationSet.id)
                    {
                        if (transition.first.isTerminal)
                        {
                            std::cout << "Error: conflict in action table" << std::endl;
                            std::cout << "State: " << configurationSets[i].id << std::endl;
                            std::cout << "Symbol: " << transition.first.humanReadableName << std::endl;
                            std::cout << "Existing action: " << targetTable[std::make_pair(configurationSets[i].id, transition.first.id)] << std::endl;
                            std::cout << "New action: " << newConfigurationSet.id << std::endl;
                        }
                        else
                        {
                            std::cout << "Error: conflict in goto table" << std::endl;
                            std::cout << "State: " << configurationSets[i].id << std::endl;
                            std::cout << "Symbol: " << transition.first.humanReadableName << std::endl;
                            std::cout << "Existing goto: " << targetTable[std::make_pair(configurationSets[i].id, transition.first.id)] << std::endl;
                            std::cout << "New goto: " << newConfigurationSet.id << std::endl;
                        }

                        printConfigurationSets();
                        printActionTable();
                        printGotoTable();
                        exit(1);
                    }
                }
            }
        }
        if (!changed)
        {
            break;
        }
    }
    // now consider the reduction case
    for (const auto &configurationSet : configurationSets)
    {
        for (const auto &configuration : configurationSet.configurations)
        {
            if (configuration.isComplete())
            {

                for (const auto &configLookahead : configuration.lookaheads)
                {
                    if (actionTable.count(std::make_pair(configurationSet.id, configLookahead.id)) != 0)
                    {
                        bool tolerate = false;
                        if (actionTable[std::make_pair(configurationSet.id, configLookahead.id)] > 0)
                        {
                            std::cout << "Warning: shift/reduce conflict, use shift instead" << std::endl;
                            std::cout << "Ignore possible shift by rule: " << configuration.production->lhs.humanReadableName << " -> ";
                            for (const auto &symbol : configuration.production->rhs)
                            {
                                std::cout << symbol.humanReadableName << " ";
                            }
                            std::cout << std::endl;
                            tolerate = true;
                        }
                        else
                        {
                            std::cout << "Error: reduce/reduce conflict" << std::endl;
                        }

                        if (!tolerate)
                        {
                            exit(1);
                        }
                        else
                        {
                            continue;
                        }
                    }
                    actionTable[std::make_pair(configurationSet.id, configLookahead.id)] = -configuration.production->id;
                }
            }
        }
    }
}

/// @brief parse the input from the lexer
/// @details this is the core function of the parser, it parses the input from the lexer with the action table and goto table
/// @param input the input from the lexer
bool Parser::parse(std::vector<Symbol> input)
{
    std::vector<int> stack;
    std::vector<Symbol> symbolStack;
    stack.push_back(0);
    input.push_back(endSymbol);
    size_t inputCursor = 0;
    while (inputCursor < input.size())
    {
        int currentState = stack.back();
        Symbol currentSymbol = input[inputCursor];
        if (actionTable.count(std::make_pair(currentState, currentSymbol.id)) == 0)
        {
            // print error message
            std::cout << "Error: unexpected symbol " << currentSymbol.humanReadableName << " at position " << inputCursor << std::endl;
            // print current state
            std::cout << "Current state: " << currentState << std::endl;
            return false;
        }
        int nextState = actionTable[std::make_pair(currentState, currentSymbol.id)];
        if (nextState > 0)
        {
            stack.push_back(nextState);
            symbolStack.push_back(currentSymbol);
            inputCursor++;
            // print shift message
            std::cout << "state: " << currentState << "\t";
            std::cout << "next type: " << currentSymbol.humanReadableName << "\t";
            std::cout << "shift to state " << nextState << std::endl;
            std::cout << "current situation: ";
            // print current situation according to the stack
            for (size_t i = 0; i < symbolStack.size(); i++)
            {
                std::cout << symbolStack[i].humanReadableName << " ";
            }
            std::cout << "|" << std::endl;
        }
        else if (nextState < 0)
        {
            int production = -nextState;
            // print reduce message

            // pop series of states from the stack according to the rhs of the production
            for (size_t j = 0; j < productions[production]->rhs.size(); j++)
            {
                stack.pop_back();
                symbolStack.pop_back();
            }

            if (gotoTable.count(std::make_pair(stack.back(), productions[production]->lhs.id)) == 0)
            {
                // print error message
                std::cout << "Error: unexpected symbol " << currentSymbol.humanReadableName << " at position " << inputCursor << std::endl;
                // print current state
                std::cout << "Current state: " << currentState << std::endl;
                return false;
            }

            // push the next state according to the goto table
            nextState = gotoTable[std::make_pair(stack.back(), productions[production]->lhs.id)];

            stack.push_back(nextState);
            symbolStack.push_back(productions[production]->lhs);

            /*
            Sample shift message
            state: 49	next type: SEMI		reduce by grammar 9: declaration->ID
            current situation: INT | declaration
            */
            std::cout << "state: " << currentState << "\t";
            std::cout << "next type: " << currentSymbol.humanReadableName << "\t";
            std::cout << "reduce by grammar " << production << ": " << productions[production]->lhs.humanReadableName << "->";
            for (size_t j = 0; j < productions[production]->rhs.size(); j++)
            {
                std::cout << productions[production]->rhs[j].humanReadableName << " ";
            }
            if (productions[production]->rhs.size() == 0)
            {
                std::cout << "{}";
            }
            std::cout << std::endl;
            std::cout << "current situation: ";
            // print current situation according to the stack
            for (int i = 0; i < int(symbolStack.size()) - 1; i++)
            {
                std::cout << symbolStack[i].humanReadableName << " ";
            }
            std::cout << "| " << symbolStack.back().humanReadableName << std::endl;
        }
        else
        {
            std::cout << "reduce by augmented grammar: " << productions[0]->lhs.humanReadableName << "->";
            for (size_t j = 0; j < productions[0]->rhs.size(); j++)
            {
                std::cout << productions[0]->rhs[j].humanReadableName << " ";
            }
            std::cout << std::endl
                      << std::endl;
            std::cout << "Accepted" << std::endl;
            return true;
        }
        std::cout << std::endl;
    }
    return false;
}

/// @brief add a production to the parser
/// @details this function adds a production to the parser, it also updates the nullable symbols
/// @param lhs the left hand side of the production
/// @param rhs the right hand side of the production
/// @return the id of the production
int Parser::addProduction(Symbol lhs, std::vector<Symbol> rhs)
{
    auto production = std::make_shared<Production>(lhs, rhs, productions.size());
    if (production->rhs.size() == 0)
    {
        nullableSymbols.insert(production->lhs.id);
    }
    productions.push_back(production);
    return production->id;
}

/// @brief get the first set of a symbol
/// @details this function returns the first set of a symbol
/// @param symbol the symbol
/// @return a hashed set of symbols
std::unordered_set<Symbol> Parser::getFirstSet(Symbol symbol)
{

    if (symbol.isTerminal)
    {
        return {symbol};
    }
    if (firstSetMemo.count(symbol) != 0)
    {
        return firstSetMemo.at(symbol);
    }
    firstSetRecursionMemo.insert(symbol);
    std::unordered_set<Symbol> firstSet;
    for (const auto &production : productions)
    {
        if (production->lhs == symbol)
        {
            for (size_t i = 0; i < production->rhs.size(); i++)
            {
                if (production->rhs[i].isTerminal)
                {
                    firstSet.insert(production->rhs[0]);
                }
                else
                {
                    if (firstSetRecursionMemo.count(production->rhs[i]) == 0)
                    {
                        std::unordered_set<Symbol> firstSetOfRhs = getFirstSet(production->rhs[i]);
                        firstSet.insert(firstSetOfRhs.begin(), firstSetOfRhs.end());
                    }
                }
                if (nullableSymbols.count(production->rhs[i].id) == 0)
                {
                    break;
                }
            }
        }
    }
    firstSetRecursionMemo.erase(symbol);
    firstSetMemo[symbol] = firstSet;
    return firstSet;
}

/// @brief get the first set of a vector of symbols
/// @details this function returns the first set of a vector of symbols
/// @param symbols the vector of symbols
/// @return a hash set that contains the first set of the vector of symbols
std::unordered_set<Symbol> Parser::getFirstSet(const std::vector<Symbol> &symbols)
{
    std::unordered_set<Symbol> firstSet;
    for (const auto &symbol : symbols)
    {
        std::unordered_set<Symbol> firstSetOfSymbol = getFirstSet(symbol);
        firstSet.insert(firstSetOfSymbol.begin(), firstSetOfSymbol.end());
        if (nullableSymbols.count(symbol.id) == 0)
        {
            break;
        }
    }
    return firstSet;
}

/// @brief print the productions
void Parser::printProductions() const
{
    for (const auto &production : productions)
    {
        std::cout << production->id << ": ";
        std::cout << production->lhs.humanReadableName << " -> ";
        for (const auto &symbol : production->rhs)
        {
            std::cout << symbol.humanReadableName << " ";
        }
        std::cout << std::endl;
    }
}

/// @brief print a particular configuration set
void Parser::printConfigurationSet(const ConfigurationSet &configurationSet) const
{
    std::cout << "Configuration Set " << configurationSet.id << std::endl;
    for (const auto &configuration : configurationSet.configurations)
    {
        std::cout << configuration.production->lhs.humanReadableName << " -> ";
        for (size_t i = 0; i < configuration.production->rhs.size(); i++)
        {
            if (int(i) == configuration.dotPosition)
            {
                std::cout << ". ";
            }
            std::cout << configuration.production->rhs[i].humanReadableName << " ";
        }
        if (configuration.dotPosition == int(configuration.production->rhs.size()))
        {
            std::cout << ".";
        }
        std::cout << " , ";
        for (const auto &lookahead : configuration.lookaheads)
        {
            std::cout << lookahead.humanReadableName << " ";
        }
        std::cout << std::endl;
    }
}

/// @brief print configuration sets of the parser
void Parser::printConfigurationSets() const
{
    for (const auto &configurationSet : configurationSets)
    {
        printConfigurationSet(configurationSet);
    }
}

/// @brief print the action table
void Parser::printActionTable() const
{
    std::cout << "Action Table" << std::endl;
    for (const auto &entry : actionTable)
    {
        std::cout << entry.first.first << " " << entry.first.second << " " << entry.second << std::endl;
    }
}

/// @brief print the goto table
void Parser::printGotoTable() const
{
    std::cout << "Goto Table" << std::endl;
    for (const auto &entry : gotoTable)
    {
        std::cout << entry.first.first << " " << entry.first.second << " " << entry.second << std::endl;
    }
}