#include "configurationSet.h"
#include "configuration.h"
#include <vector>

ConfigurationSet::ConfigurationSet(std::vector<Configuration> configurations, int id) : configurations{configurations.begin(), configurations.end()}, id{id} {
    
}

bool ConfigurationSet::operator==(const ConfigurationSet &other) const
{
    return configurations == other.configurations;
}