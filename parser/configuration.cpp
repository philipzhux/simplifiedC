#include "configuration.h"

Configuration::Configuration(std::shared_ptr<Production> production, int dotPosition, std::unordered_set<Symbol> lookaheads) : production(production), dotPosition(dotPosition), lookaheads(lookaheads) {}

std::pair<Symbol, Configuration> Configuration::getTransition()
{
    assert(!isComplete());
    return std::make_pair(production->rhs[dotPosition], Configuration(production, dotPosition + 1, lookaheads));
}

bool Configuration::operator==(const Configuration &other) const
{
    return *production == *other.production && dotPosition == other.dotPosition && lookaheads == other.lookaheads;
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
    return controlId(true);
}

void ConfigurationSet::rollbackId()
{
    controlId(false);
}

int ConfigurationSet::controlId(bool increment)
{
    static int id = 0;
    if (increment)
    {
        return id++;
    }
    else
    {
        id--;
        return id;
    }
}