#pragma once
#include "scanner.hpp"
#include <memory>
#include <cereal/types/memory.hpp>
#include <unordered_set>
#include <cereal/types/unordered_set.hpp>
// non-linux systems need <string> as not always included by <unordered_set>
#include <string>
#include "ast/syntax_tree.hpp"

/// @brief A symbol is a terminal or non-terminal symbol.
class Symbol
{
public:
    Symbol();
    Symbol(std::string name, bool isTerminal, int id);
    Symbol(std::string name, bool isTerminal, int id, int intValue);
    Symbol(std::string name, bool isTerminal, int id, std::string stringValue);
    bool operator==(const Symbol &other) const;
    std::string humanReadableName;
    int intValue;
    std::string stringValue;
    bool isTerminal;
    static int getId();
    int id;
    std::shared_ptr<ASTGen::SyntaxTreeNode> syntaxTreeNode;
    template<class Archive>
    void serialize(Archive & archive)
    {
        archive( CEREAL_NVP(humanReadableName), CEREAL_NVP(isTerminal), CEREAL_NVP(id) ); // serialize things by passing them to the archive
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
    Terminal(std::string name, Token token, int id, int intValue);
    Terminal(std::string name, Token token, int id, std::string stringValue);
private:
    Token token;
};

/// @brief A non-terminal symbol.
class NonTerminal : public Symbol
{
public:
    NonTerminal(std::string name, int id);
};

