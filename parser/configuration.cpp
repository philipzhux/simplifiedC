#include "configuration.h"

Configuration::Configuration(std::shared_ptr<Production> production, int dotPosition, Symbol lookahead) : production(production), dotPosition(dotPosition), lookahead(lookahead) {}

std::pair<Symbol, Configuration> Configuration::getTransition()
{
    assert(!isComplete());
    return std::make_pair(production->rhs[dotPosition], Configuration(production, dotPosition + 1, lookahead));
}

bool Configuration::operator==(const Configuration &other) const
{
    return *production == *other.production && dotPosition == other.dotPosition && lookahead == other.lookahead;
}

// This is a function for checking if the configuration is complete.
bool Configuration::isComplete()
{
    return dotPosition == production->rhs.size();
}

// This is a function to get the symbol after dot.
Symbol Configuration::getSymbolAfterDot()
{
    return production->rhs[dotPosition];
}

// This is a function to get the closure of the configuration.
std::vector<Configuration> Configuration::closure(const Parser &parser)
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