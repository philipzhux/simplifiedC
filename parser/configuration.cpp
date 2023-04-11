#include "configuration.h"

Configuration::Configuration(std::shared_ptr<Production> production, int dotPosition, std::unordered_set<Symbol> lookaheads) : production(production), dotPosition(dotPosition), lookaheads(lookaheads) {}

/// @brief Get the transition symbol and the next configuration.
/// @return A pair of the transition symbol and the next configuration.
std::pair<Symbol, Configuration> Configuration::getTransition() const
{
    assert(!isComplete());
    return std::make_pair(production->rhs[dotPosition], Configuration(production, dotPosition + 1, lookaheads));
}

bool Configuration::operator==(const Configuration &other) const
{
    return *production == *other.production && dotPosition == other.dotPosition && lookaheads == other.lookaheads;
}

/// @brief Check if the configuration is complete.
/// @return True if the configuration is complete, false otherwise.
bool Configuration::isComplete() const
{
    return dotPosition == production->rhs.size();
}


/// @brief Get the symbol after the dot.
/// @return The symbol after the dot.
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

/// @brief Get an auto-incremented id.
int ConfigurationSet::getId()
{
    return controlId(true);
}

/// @brief Rollback the id when a temporary configuration set is not committed.
void ConfigurationSet::rollbackId()
{
    controlId(false);
}

/// @brief Core control function for id manipulation.
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