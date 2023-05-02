#pragma once
#include "symbol.h"
#include <vector>
#include <cereal/types/vector.hpp>

/// @brief A production is a rule in the grammar.
/// @details The production is defined by a left-hand side symbol and a right-hand side sequence of symbols.
class Production
{
public:
    Production();
    Production(Symbol lhs, std::vector<Symbol> rhs, int id, std::function<std::shared_ptr<ASTGen::SyntaxTreeNode>(std::vector<std::shared_ptr<ASTGen::SyntaxTreeNode>>)> action);
    int id;
    Symbol lhs;
    std::vector<Symbol> rhs;
    bool operator==(const Production &other) const;
    std::function<std::shared_ptr<ASTGen::SyntaxTreeNode>(std::vector<std::shared_ptr<ASTGen::SyntaxTreeNode>>)> action;
    static int getId();

    template<class Archive>
    void serialize(Archive & archive)
    {
        archive( CEREAL_NVP(lhs), CEREAL_NVP(rhs), CEREAL_NVP(id) ); // serialize things by passing them to the archive
    }
};