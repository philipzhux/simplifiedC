#pragma once

class Symbol;
class Configuration;

class ConfigurationSet
{
public:
    ConfigurationSet(std::vector<Configuration> configurations, int id);
    static int getId()
    {
        static int id = 0;
        return id++;
    }
    bool operator==(const ConfigurationSet &other) const;
    std::unordered_set<Configuration> configurations;
    int id;
};
