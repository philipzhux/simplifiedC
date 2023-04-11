#pragma once
#include "../scanner/scanner.hpp"
#include <memory>
#include <cereal/types/memory.hpp>
#include <unordered_set>
#include <cereal/types/unordered_set.hpp>
// non-linux systems need <string> as not always included by <unordered_set>
#include <string>

/// @brief A symbol is a terminal or non-terminal symbol.
class Symbol
{
public:
    Symbol();
    Symbol(std::string name, bool isTerminal, int id);
    bool operator==(const Symbol &other) const;
    bool isTerminal;
    std::string humanReadableName;
    static int getId();
    int id;
    template<class Archive>
    void serialize(Archive & archive)
    {
        archive( humanReadableName, isTerminal, id ); // serialize things by passing them to the archive
    }
};

// hash function for Symbol
namespace std
{
    template <>
    struct hash<Symbol>
    {
        size_t operator()(const Symbol &symbol) const
        {
            return hash<int>()(symbol.id);
        }
    };
}

/// @brief A terminal symbol.
class Terminal : public Symbol
{
public:
    Terminal();
    Terminal(std::string name, Token token, int id);

private:
    Token token;
};

/// @brief A non-terminal symbol.
class NonTerminal : public Symbol
{
public:
    NonTerminal(std::string name, int id);
};

