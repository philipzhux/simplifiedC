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
    assert(!isComplete());
    return production->rhs[dotPosition];
}





ConfigurationSet::ConfigurationSet(std::vector<Configuration> configurations, int id) : configurations{configurations.begin(), configurations.end()}, id{id} {
    
}

bool ConfigurationSet::operator==(const ConfigurationSet &other) const
{
    return configurations == other.configurations;
}
int ConfigurationSet::getId()
{
    static int id = 0;
    return id++;
}